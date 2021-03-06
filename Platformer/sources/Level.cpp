#include "Level.h"

#include <algorithm>
#include <SDL.h>
#include <json/json.h>


#include "ImageLibrary.h"
#include "LoadHelper.h"
#include "Game.h"

using namespace Platformer;


void Level::setRenderer(SDL_Renderer* pRenderer)
{
	mRenderer = pRenderer;
}

void Level::setGame(Game* pGame)
{
	mGame = pGame;
}

void Level::init()
{
	mLevelToLoad = "";
	mScreenSize = Size(800, 600);
	mRenderTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mScreenSize.width, mScreenSize.height);
	mScreenHitBox.width = mScreenSize.width;
	mScreenHitBox.height = mScreenSize.height;

	mCamera.setScreenSize(mScreenSize);
}

void Level::update(unsigned int pTicks)
{
	mAreaManager.updateItems(&mItems);
	
	unsigned int lTicks = SDL_GetTicks();
	updateOnScreenValues();
	activeItems();
	for (auto &lItem : mItems)
	{
		if (lItem->getActive())
		{
			lItem->update(pTicks);
		}
	}

	removeItems();
	addItems();
	unsigned int lDiff = SDL_GetTicks() - lTicks;

	mCamera.update(pTicks);
	updateScreenHitBox();
	
	if (mLevelToLoad != "")
	{
		mGame->startPlatformLevel(mLevelToLoad);
	}

}

void Level::activeItems()
{
	for (auto &lItem : mItems)
	{
		if (!lItem->getActive() && lItem->getIsOnScreen())
		{
			lItem->active();
		}
	}
}

void Level::addItems()
{
	for (auto &lItemToAdd : mItemsToAdd)
	{
		mItems.push_back(lItemToAdd);
	}
	mItemsToAdd.clear();

	for (auto& lItemToAdd : mItemsToAddToBack)
	{
		mItems.push_front(lItemToAdd);
	}
	mItemsToAddToBack.clear();
}

void Level::removeItems()
{
	for (auto &lItemToRemove : mItemsToRemove)
	{
		if (lItemToRemove->typeName() == "PlayerAvatar")
		{
			mPlayerAvatars.remove((PlayerAvatar*)lItemToRemove);
		}
		mItems.remove(lItemToRemove);
		delete lItemToRemove;
	}
	mItemsToRemove.clear();
}

void Level::updateScreenHitBox()
{
	mScreenHitBox.x = mCamera.getShift().x * -1;
	mScreenHitBox.y = mCamera.getShift().y * -1;
}

void Level::updateOnScreenValues()
{
	for (auto &lItem : mItems)
	{
		lItem->updateOnScreenValue(mScreenHitBox);
	}
}

void Level::draw(SDL_Renderer *pSDL_Renderer, unsigned int pTicks)
{
	SDL_SetRenderTarget(mRenderer, mRenderTexture);

	SDL_SetRenderDrawColor(pSDL_Renderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 255);
	SDL_RenderClear(pSDL_Renderer);

	mBackDecorsMap->draw(pSDL_Renderer, mCamera.getShift(), mScreenHitBox);
	for (auto &lItem : mItems)
	{
		if (!lItem->getIsOnScreen())
		{
			continue;
		}
		lItem->draw(pSDL_Renderer, mCamera.getShift(), pTicks);
	}
	mFrontDecorsMap->draw(pSDL_Renderer, mCamera.getShift(), mScreenHitBox);
}

Size Level::getScreenSize()
{
	return mScreenSize;
}

Map* Level::getCollisionMap()
{
	return mCollisionMap;
}

void Level::uninit()
{
	for (auto &lItem : mItems)
	{
		delete lItem;
	}
}

std::list<Item*>* Level::getItems()
{
	return &mItems;
}

std::list<PlayerAvatar*> *Level::getPlayerAvatars()
{
	return &mPlayerAvatars;
}

void Level::removeItem(Item *pItem)
{
	mItemsToRemove.push_back(pItem);
}

void Level::addItem(Item *pItem)
{
	mItemsToAdd.push_back(pItem);
	if (pItem->typeName() == "PlayerAvatar")
	{
		mPlayerAvatars.push_back((PlayerAvatar*)pItem);
	}
}

void Level::addItemToBack(Item *pItem)
{
	mItemsToAddToBack.push_back(pItem);
}

SDL_Texture* Level::getRenderTexture()
{
	return mRenderTexture;
}

void Level::setLevelToLoad(std::string pLevelPath, Point pAvatarPosition)
{
	mLevelToLoad = pLevelPath;
	mReloadAvatarPosition = pAvatarPosition;
}

Level *Level::LoadFromFile(std::string pFilePath, Point pAvatarPosition)
{
	Level *lLevelResult = new Level();
	lLevelResult->init();

	std::string lStrJson = loadTextFile(pFilePath);

	Json::Value lRoot;
	Json::Reader lReader;

	bool lParsingSuccessful = lReader.parse(lStrJson, lRoot, false);
	if (!lParsingSuccessful)
	{
		return nullptr;
	}

	lLevelResult->mBackgroundColor.r = lRoot["BackColorR"].asInt();
	lLevelResult->mBackgroundColor.g = lRoot["BackColorG"].asInt();
	lLevelResult->mBackgroundColor.b = lRoot["BackColorB"].asInt();

	Json::Value lJsonMaps = lRoot["Maps"];
	for (Json::ValueIterator lItrJsonMap = lJsonMaps.begin(); lItrJsonMap != lJsonMaps.end(); lItrJsonMap++) 
	{
		Map *lNewMap = Map::LoadFromJson(*lItrJsonMap);
		if (lNewMap->getName() == "CollisionMap")
		{
			lLevelResult->mCollisionMap = lNewMap;
		}
		else if (lNewMap->getName() == "BackDecorsMap")
		{
			lLevelResult->mBackDecorsMap = lNewMap;
		}
		else if (lNewMap->getName() == "FrontDecorsMap")
		{
			lLevelResult->mFrontDecorsMap = lNewMap;
		}
	}

	Json::Value lJsonItems = lRoot["Items"];
	for (Json::ValueIterator lItrJsonItem = lJsonItems.begin(); lItrJsonItem != lJsonItems.end(); lItrJsonItem++)
	{
		Json::Value lJsonItem = (*lItrJsonItem);

		std::string lItemName = lJsonItem["Name"].asString();
		Item* lNewItem = Item::createItem(lItemName);
		if (lNewItem->typeName() == "PlayerAvatar")
		{
			if (pAvatarPosition.x != -1 )
			{
				lNewItem->setPosition(pAvatarPosition);
			}
		}

		lNewItem->setLevel(lLevelResult);
		lNewItem->init(lJsonItem);
		lLevelResult->addItem(lNewItem);
	}

	lLevelResult->mCamera.setCollistionMap(lLevelResult->mCollisionMap);
	lLevelResult->mCamera.setLevel(lLevelResult);
	
	Size lLevelSize = lLevelResult->mBackDecorsMap->getSize();
	int lTileSize = lLevelResult->mBackDecorsMap->getTileSize();
	lLevelResult->mAreaManager.setLevelSize(Size(lLevelSize.width * lTileSize, lLevelSize.height * lTileSize));
	lLevelResult->mAreaManager.init();

	return lLevelResult;
}
