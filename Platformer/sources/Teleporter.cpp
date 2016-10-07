#include "Teleporter.h"
#include "SoundPlayer.h"
#include "Level.h"

using namespace Platformer;

void Teleporter::init()
{
	setHitBox(Rectangle(16, 16, 0, -1));

	mSpeed = FPoint(0.0f, 0.0f);
}

void Teleporter::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));

	mAvatarPosition.x = pJson["AvatarPositionX"].asInt();
	mAvatarPosition.y = pJson["AvatarPositionY"].asInt();
	if (pJson.isMember("LevelPath"))
	{
		mLevelPath = pJson["LevelPath"].asString();
	}
	else
	{
		mLevelPath = "";
	}
}

void Teleporter::update(unsigned int pTicks)
{
	Item::update(pTicks);
}

void Teleporter::onAvatarProximity(unsigned int pTime, Item* pAvatar)
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

std::string Teleporter::typeName()
{
	return "Teleporter";
}

Item* Teleporter::createItem()
{
	return new Teleporter();
}
