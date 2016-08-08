#include "Map.h"

using namespace Platformer;

bool Map::mCollisionsFunctionsLoaded = false;
std::map<TileCollision, std::function<bool(const Point &, int, const Rectangle &, const FPoint &)>> Map::mCollisionsFunctions;

void Map::initCollisionsFunctions()
{
	if (mCollisionsFunctionsLoaded)
	{
		return;
	}

	mCollisionsFunctionsLoaded = true;

	using namespace std::placeholders;
	mCollisionsFunctions[TileCollision::TileCollision_None]			= std::bind(Map::testTileCollisionNone, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Complete]		= std::bind(Map::testTileCollisionComplete, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Angle45]		= std::bind(Map::testTileCollisionAngle45, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Angle135]		= std::bind(Map::testTileCollisionAngle135, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Angle22_1]	= std::bind(Map::testTileCollisionAngle22_1, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Angle22_2]	= std::bind(Map::testTileCollisionAngle22_2, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Angle112_1]	= std::bind(Map::testTileCollisionAngle112_1, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_Angle112_2]	= std::bind(Map::testTileCollisionAngle112_2, _1, _2, _3, _4);
	mCollisionsFunctions[TileCollision::TileCollision_OneWay]		= std::bind(Map::testTileCollisionOneWay, _1, _2, _3, _4);
}

bool Map::testTileCollisionNone(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	return false;
}

bool Map::testTileCollisionComplete(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	return true;
}

bool Map::testTileCollisionAngle45(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lRelativeButtomRightX = (pHitbox.x + (pHitbox.width + 1)) - (pTilePosition.x * pTileSize);
	int lRelativeButtomRightY = (pHitbox.y + (pHitbox.height + 1)) - (pTilePosition.y * pTileSize);

	return lRelativeButtomRightX + lRelativeButtomRightY > pTileSize;
}

bool Map::testTileCollisionAngle135(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lRelativeButtomLeftX = pHitbox.x - (pTilePosition.x * pTileSize);
	int lRelativeButtomLeftY = (pHitbox.y + pHitbox.height) - (pTilePosition.y * pTileSize);

	return lRelativeButtomLeftX < lRelativeButtomLeftY;
}

bool Map::testTileCollisionAngle22_1(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lRelativeButtomRightX = (pHitbox.x + (pHitbox.width + 1)) - (pTilePosition.x * pTileSize);
	int lRelativeButtomRightY = (pHitbox.y + (pHitbox.height + 1)) - (pTilePosition.y * pTileSize);

	if (lRelativeButtomRightX <= pTileSize)
	{
		if ((lRelativeButtomRightX / 2) + lRelativeButtomRightY > pTileSize)
		{
			return true;
		}
	}
	else if (lRelativeButtomRightY > pTileSize / 2)
	{
		return true;
	}
	return false;
}

bool Map::testTileCollisionAngle22_2(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lRelativeButtomRightX = (pHitbox.x + (pHitbox.width + 1)) - (pTilePosition.x * pTileSize);
	int lRelativeButtomRightY = (pHitbox.y + (pHitbox.height + 1)) - (pTilePosition.y * pTileSize);

	if (lRelativeButtomRightX <= pTileSize)
	{
		if ((lRelativeButtomRightX / 2) + lRelativeButtomRightY > pTileSize / 2)
		{
			return true;
		}
	}
	else if (lRelativeButtomRightY > 0)
	{
		return true;
	}
	return false;
}

bool Map::testTileCollisionAngle112_1(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lRelativeButtomLeftX = pHitbox.x - (pTilePosition.x * pTileSize);
	int lRelativeButtomLeftY = (pHitbox.y + pHitbox.height) - (pTilePosition.y * pTileSize);

	if (lRelativeButtomLeftX >= 0)
	{
		if (lRelativeButtomLeftX < lRelativeButtomLeftY * 2)
		{
			return true;
		}
	}
	else if (lRelativeButtomLeftY > pTileSize)
	{
		return true;
	}
	return false;
}

bool Map::testTileCollisionAngle112_2(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lRelativeButtomLeftX = pHitbox.x - (pTilePosition.x * pTileSize);
	int lRelativeButtomLeftY = (pHitbox.y + pHitbox.height) - (pTilePosition.y * pTileSize);

	if (lRelativeButtomLeftX >= 0)
	{
		if (lRelativeButtomLeftX < (lRelativeButtomLeftY * 2) - pTileSize)
		{
			return true;
		}
	}
	else if (lRelativeButtomLeftY > pTileSize / 2)
	{
		return true;
	}
	return false;
}

bool Map::testTileCollisionOneWay(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection)
{
	int lButtom = (pHitbox.y + pHitbox.height);
	return (lButtom == (pTilePosition.y * pTileSize)) && pDirection.y >= 0.0f;
}
