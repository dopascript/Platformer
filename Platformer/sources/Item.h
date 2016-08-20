#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <SDL.h>
#include <json/json.h>

#include "Sprite.h"
#include "Point.h"
#include "FPoint.h"
#include "Color.h"

namespace Platformer
{
	class Level;
	class Area;

	enum CollisionTest { CollisionTest_None, CollisionTest_MapAndItems, CollisionTest_Map, CollisionTest_Items };

	class Item
	{
	public:
		Item();

		void setLevel(Level *pLevel);

		void setSprite(Sprite &pSprite);
		Sprite* getSprite();

		void setPosition(Point pPosition);
		Point getPosition();

		void setHitBox(Rectangle pHitBox);
		Rectangle getHitBox();

		void setSpeed(FPoint pSpeed);
		FPoint getSpeed();

		void setIsSolid(bool pIsSolid);
		bool getIsSolid();

		void active();
		bool getActive();

		void updateOnScreenValue(Rectangle &mScreenHitBox);
		bool getIsOnScreen();

		bool testHit(Point pPosition, FPoint pDirection, CollisionTest pCollisionTest);

		Rectangle getAbsolutHitBox();
		Rectangle getAbsolutHitBox(Point pPosition);
		bool testHitMap(Point pPosition, FPoint pDirection);
		std::vector<Item*> findHitItems(Point pPosition);
		bool testHitSolidItems(Point pPosition);

		void move(FPoint pDirection, CollisionTest pCollisionTest);

		virtual void init(Json::Value pJson) = 0;
		virtual void update(unsigned int pTicks) = 0;

		void draw(SDL_Renderer *pSDL_Renderer, Point pCameraShift, int pTime);

		virtual std::string typeName() = 0;
		virtual Item* createItem() = 0;

		static void addItemsType(Item *pItem);
		static Item* createItem(std::string pName);
		static bool itemTypeExist(std::string pName);
		static void itemsTypesListing();

		void addArea(Area* pArea);
		void clearArea();
		void updateItemsInArea();
	protected:
		Level *mLevel;
		Sprite mSprite;
		Point mPosition;
		Rectangle mHitBox;
		FPoint mSpeed;
		bool mIsSolid;
		bool mIsOnScreen;

		Color mDrawColor;
		bool mActive;

		std::vector<Area*> mAreas;
		std::vector<Item*> mItemsInArea;
		static std::map<std::string, Item*> mItemsTypes;
	};
}

#endif