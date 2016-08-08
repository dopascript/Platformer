#ifndef MAP_H
#define MAP_H

#include <map>
#include <SDL.h>
#include <functional>
#include <json/json.h>
#include "Point.h"
#include "FPoint.h"
#include "Size.h"
#include "Rectangle.h"
namespace Platformer
{
	enum TileCollision { TileCollision_None, TileCollision_Complete, TileCollision_Angle45, TileCollision_Angle135, TileCollision_Angle22_1, TileCollision_Angle22_2, TileCollision_Angle112_1, TileCollision_Angle112_2, TileCollision_OneWay };

	class Map
	{
	public:
		Map();
		Map(const Map& pOther);
		~Map();
		void setMap(std::string pMapFile);

		std::string getName() const;
		Size getSize() const;
		int getTileSize() const;
		int* getTiles() const;
		SDL_Texture* getTexture() const;
		TileCollision getTileCollisionAtPosition(Point pPosition);

		bool testHit(Point pPosition, FPoint pDirection);
		bool testHit(Rectangle pHitBox, FPoint pDirection);

		void draw(SDL_Renderer* pSDL_Renderer, Point &pCameraShift, Rectangle &pScreenHitBox);

		static Map* LoadFromJson(Json::Value pJsonMap);
	private:
		std::string mName;
		Size mSize;
		int* mTiles;
		SDL_Texture* mTexture;
		int mTileSize;
		int mTextureTileSize;
		int mTextureTilesCountY;

		static bool mCollisionsFunctionsLoaded;
		static std::map<TileCollision, std::function<bool(const Point &, int, const Rectangle &, const FPoint &)>> mCollisionsFunctions;

		static void initCollisionsFunctions();
		static bool testTileCollisionNone(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionComplete(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionAngle45(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionAngle135(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionAngle22_1(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionAngle22_2(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionAngle112_1(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionAngle112_2(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
		static bool testTileCollisionOneWay(const Point &pTilePosition, int pTileSize, const Rectangle &pHitbox, const FPoint &pDirection);
	};
}
#endif