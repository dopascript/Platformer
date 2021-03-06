#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <list>
#include <SDL.h>
#include <json/json.h>

#include "Sprite.h"
#include "Point.h"
#include "FPoint.h"
#include "Color.h"

#define ITEMS_PROXIMITY_MAX 100

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
		virtual void update(unsigned int pTicks);
		void updateRemoveAnimation(unsigned int pTicks);

		void draw(SDL_Renderer *pSDL_Renderer, Point pCameraShift, int pTime);

		virtual std::string typeName() = 0;
		virtual Item* createItem() = 0;

		static void addItemsType(Item *pItem);
		static Item* createItem(std::string pName);
		static bool itemTypeExist(std::string pName);
		static void itemsTypesListing();

		void addItemProximity(Item* pItem);
		void clearItemsProximity();

		virtual void onAvatarProximity(unsigned int pTime, Item* pAvatar);
		virtual void onMobileItemProximity(unsigned int pTime, Item* pItem);

		void startRemoveAnimation(unsigned int pTime);
	protected:
		Level *mLevel;
		Sprite mSprite;
		Point mPosition;
		Rectangle mHitBox;
		FPoint mSpeed;
		bool mIsSolid;
		bool mIsOnScreen;
		bool mNeedListItemsProximity;
		Color mDrawColor;
		bool mActive;
		bool mRemoveAnimation;
		int mRemoveAnimationStartTime;
		bool mMobile;

		Item* mItemsProximity[ITEMS_PROXIMITY_MAX];
		int mItemsProximityCount;
		static std::map<std::string, Item*> mItemsTypes;
	};
}

#endif