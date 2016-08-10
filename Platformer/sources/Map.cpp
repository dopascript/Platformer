#include "Map.h"

#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

#include "ImageLibrary.h"
#include "LoadHelper.h"

using namespace Platformer;

Map::Map()
{
	mTiles = nullptr;
	initCollisionsFunctions();
}

Map::Map(const Map &pOther)
{
	mSize = pOther.getSize();
	mTiles = new int[mSize.width, mSize.height];
	memcpy(mTiles, pOther.getTiles(), mSize.width * mSize.height * sizeof(int));
}

Map::~Map()
{
	if (mTiles != nullptr)
	{
		delete mTiles;
	}
}

void Map::setMap(std::string pMapFile)
{
	std::ifstream infile(pMapFile.c_str());

	std::string lLine;

	std::getline(infile, lLine);
	mTexture = ImageLibrary::getInstance()->getImage(lLine);

	std::getline(infile, lLine);
	int lWidth = std::atoi(lLine.c_str());

	std::getline(infile, lLine);
	int lHeight = std::atoi(lLine.c_str());
	
	mSize = Size(lWidth, lHeight);

	mTiles = new int[lWidth * lHeight];

	for (int lY = 0; lY < lHeight; lY++)
	{
		std::getline(infile, lLine);
		for (int lX = 0; lX < lWidth; lX++)
		{
			int value = lLine[lX] - '0';
			int lArrayPosition = (lY * lWidth) + lX;
			mTiles[lArrayPosition] = value;
		}
	}
}

std::string Map::getName() const
{
	return mName;
}

Size Map::getSize() const
{
	return mSize;
}

int* Map::getTiles() const
{
	return mTiles;
}

int Map::getTileSize() const
{
	return mTileSize;
}

SDL_Texture* Map::getTexture() const
{
	return mTexture;
}

bool Map::testHit(Point pPosition, FPoint pDirection)
{
	int lTileX = pPosition.x / mTileSize;
	int lTileY = pPosition.y / mTileSize;
	return mTiles[lTileX * mSize.width + lTileY] != 0;
}

bool Map::testHit(Rectangle pHitBox, FPoint pDirection)
{
	int lStartTileX = pHitBox.x / mTileSize;
	int lEndTileX = (pHitBox.x + pHitBox.width) / mTileSize;
	lStartTileX = std::min(std::max(lStartTileX, 0), mSize.width - 1);
	lEndTileX = std::min(std::max(lEndTileX, 0), mSize.width - 1);

	int lStartTileY = pHitBox.y / mTileSize;
	int lEndTileY = (pHitBox.y + pHitBox.height) / mTileSize;
	lStartTileY = std::min(std::max(lStartTileY, 0), mSize.height - 1);
	lEndTileY = std::min(std::max(lEndTileY, 0), mSize.height - 1);

	

	for (int lY = lStartTileY; lY <= lEndTileY; lY++)
	{
		for (int lX = lStartTileX; lX <= lEndTileX; lX++)
		{
			int value = mTiles[lY * mSize.width + lX];
			Point lTilePosition(lX, lY);
			if (mCollisionsFunctions[(TileCollision)value](lTilePosition, mTileSize, pHitBox, pDirection))
			{
				return true;
			}
		}
	}
	return false;
}

TileCollision Map::getTileCollision(Point pTilePosition)
{
	return (TileCollision)mTiles[pTilePosition.y * mSize.width + pTilePosition.x];
}

void Map::draw(SDL_Renderer *pSDL_Renderer, Point &pCameraShift, Rectangle &pScreenHitBox)
{
	SDL_Rect lTextureRectangle;
	lTextureRectangle.w = mTextureTileSize;
	lTextureRectangle.h = mTextureTileSize;
	

	SDL_Rect lOnScreenRectangle;
	lOnScreenRectangle.w = mTileSize;
	lOnScreenRectangle.h = mTileSize;

	int lStartY = pScreenHitBox.y / mTileSize;
	lStartY = std::max(lStartY, 0);
	int lEndY = ((pScreenHitBox.y + pScreenHitBox.height) / mTileSize) + 1;
	lEndY = std::min(lEndY, mSize.height);

	int lStartX = pScreenHitBox.x / mTileSize;
	lStartX = std::max(lStartX, 0);
	int lEndX = ((pScreenHitBox.x + pScreenHitBox.width) / mTileSize) + 1;
	lEndX = std::min(lEndX, mSize.width);

	for (int lY = lStartY; lY < lEndY; lY++)
	{
		for (int lX = lStartX; lX < lEndX; lX++)
		{
			int value = mTiles[lY * mSize.width + lX];
			if (value == 0)
			{
				continue;
			}
			lTextureRectangle.x = (value % mTextureTilesCountY) * mTextureTileSize;
			lTextureRectangle.y = (value / mTextureTilesCountY) * mTextureTileSize;
			lOnScreenRectangle.x = (lX * mTextureTileSize) + pCameraShift.x;
			lOnScreenRectangle.y = (lY * mTextureTileSize) + pCameraShift.y;

			SDL_RenderCopy(
				pSDL_Renderer,
				mTexture,
				&lTextureRectangle,
				&lOnScreenRectangle);
		}
	}
}



Map* Map::LoadFromJson(Json::Value pJsonMap)
{
	Map *lMapResult = new Map();

	lMapResult->mName = pJsonMap["Name"].asString();

	std::string lImagePath = pJsonMap["ImagePath"].asString();
	lMapResult->mTexture = ImageLibrary::getInstance()->getImage(lImagePath);

	int lWidth = pJsonMap["Width"].asInt();
	int lHeight = pJsonMap["Height"].asInt();

	lMapResult->mTileSize = pJsonMap["TileSize"].asInt();
	lMapResult->mTextureTileSize = pJsonMap["TileSizeInTexture"].asInt();
	lMapResult->mTextureTilesCountY = pJsonMap["TilesYCountInTexture"].asInt();

	lMapResult->mSize = Size(lWidth, lHeight);

	lMapResult->mTiles = new int[lWidth * lHeight];

	std::string lStrTiles = pJsonMap["Tiles"].asString();

	std::vector<std::string> lStrTilesSplitted = split(lStrTiles, ',');

	for (int lY = 0; lY < lHeight; lY++)
	{
		for (int lX = 0; lX < lWidth; lX++)
		{
			int lIndex = lY * lWidth + lX;
			int lTileValue = 0;
			if (lStrTilesSplitted[lIndex] != "")
			{
				lTileValue = std::atoi(lStrTilesSplitted[lIndex].c_str());
			}
			lMapResult->mTiles[lIndex] = lTileValue;
		}
	}

	return lMapResult;
}