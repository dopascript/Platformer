#include "MobilePlatform.h"
#include "ImageLibrary.h"
#include "Level.h"

using namespace Platformer;

void MobilePlatform::setStartPosition(Point pStartPosition)
{
	mStartPosition = pStartPosition;
}

void MobilePlatform::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/platform.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(16 * 4, 16));
	lPlayerSprite.setOrigin(Point(0, 0));

	setSprite(lPlayerSprite);
	setHitBox(Rectangle(16 * 4, 16, 0, -1));

	mPosition = mStartPosition;
	Point mEndPosition;

	mIsSolid = true;
	mNeedListItemsProximity = true;
}

void MobilePlatform::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));

	setSpeed(FPoint(pJson["speedX"].asInt(), pJson["speedY"].asInt()));
	setStartPosition(Point(lX, lY));

	MobilePlatformTrigger trigger1;
	trigger1.mDirection = FPoint(pJson["speedX"].asInt(), pJson["speedY"].asInt());
	trigger1.mTriggerPosition = Point(lX, lY);
	addMobilePlatformTrigger(trigger1);

	MobilePlatformTrigger trigger2;
	trigger2.mDirection = FPoint(pJson["speedX"].asInt() * -1, pJson["speedY"].asInt() * -1);
	trigger2.mTriggerPosition = Point(lX + pJson["destinationX"].asInt(), lY + pJson["destinationY"].asInt());
	addMobilePlatformTrigger(trigger2);
}

void MobilePlatform::update(unsigned int pTicks)
{
	//Check if platform must change direction
	for (auto& lTrigger : mTriggers)
	{
		if (mPosition.x == lTrigger.mTriggerPosition.x && mPosition.y == lTrigger.mTriggerPosition.y)
		{
			mSpeed = lTrigger.mDirection;
		}
	}

	//List objects on top
	std::vector<Item*> mItemsOnPlatform;
	Point lTopPosition(mPosition.x, mPosition.y - 1);
	Rectangle lTopPositionAbsHitBox = getAbsolutHitBox(lTopPosition);
	//for (auto lItem : *mLevel->getItems())
	for (int i = 0; i < mItemsProximityCount; i++)
	{
  		Item* lItem = mItemsProximity[i];
		if (this == lItem) continue;

		Rectangle lItemAbsHitBox = lItem->getAbsolutHitBox();
		if (lTopPositionAbsHitBox.testHit(lItemAbsHitBox))
		{
			mItemsOnPlatform.push_back(lItem);
		}
	}

	//Move platform
	mPosition.x = mPosition.x + (int)mSpeed.x;
	mPosition.y = mPosition.y + (int)mSpeed.y;

	//Move objets on top
	for (auto& lItem : mItemsOnPlatform)
	{
		lItem->move(mSpeed, CollisionTest_Map);
	}

	//Move colliding objects 
	Rectangle lAbsHitBox = getAbsolutHitBox();
	//for (auto lItem : *mLevel->getItems())
	for (int i = 0; i < mItemsProximityCount; i++)
	{
		Item* lItem = mItemsProximity[i];
		if (this == lItem) continue;

		Rectangle lItemAbsHitBox = lItem->getAbsolutHitBox();
		if (lItemAbsHitBox.testHit(lAbsHitBox))
		{
			lItem->move(mSpeed, CollisionTest_Map);
		}
	}
}

void MobilePlatform::addMobilePlatformTrigger(MobilePlatformTrigger pTrigger)
{
	mTriggers.push_back(pTrigger);
}

std::string MobilePlatform::typeName()
{
	return "MobilePlatform";
}

Item* MobilePlatform::createItem()
{
	return new MobilePlatform();
}