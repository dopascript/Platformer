#include "Door.h"
#include "SoundPlayer.h"
#include "Level.h"

using namespace Platformer;

void Door::init()
{
	setHitBox(Rectangle(16, 16, 0, -1));

	mSpeed = FPoint(0.0f, 0.0f);
}

void Door::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));

	mAvatarPosition.x = pJson["AvatarPositionX"].asInt();
	mAvatarPosition.y = pJson["AvatarPositionY"].asInt();
	mLevelPath = pJson["LevelPath"].asString();
}

void Door::update(unsigned int pTicks)
{
	
}

void Door::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{
	PlayerAvatar* lPlayerAvatar = (PlayerAvatar*)pAvatar;
	Rectangle lAbsPlayerHitBox = lPlayerAvatar->getAbsolutHitBox(lPlayerAvatar->getPosition());
	if (getAbsolutHitBox().testHit(lAbsPlayerHitBox))
	{
		if (mLevelPath != "")
		{
			mLevel->setLevelToLoad(mLevelPath, mAvatarPosition);
		}
		else
		{
			lPlayerAvatar->setPosition(mAvatarPosition);
		}
	}
}

std::string Door::typeName()
{
	return "Door";
}

Item* Door::createItem()
{
	return new Door();
}
