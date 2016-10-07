#include "Duplicator.h"
#include "SoundPlayer.h"
#include "Level.h"
#include "PlayerAvatar.h"

using namespace Platformer;

void Duplicator::init()
{
	setHitBox(Rectangle(16, 16, 0, -1));
	mUsed = false;
	mSpeed = FPoint(0.0f, 0.0f);
}

void Duplicator::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));

	mNewAvatarPosition.x = pJson["NewAvatarPositionX"].asInt();
	mNewAvatarPosition.y = pJson["NewAvatarPositionY"].asInt();
}

void Duplicator::update(unsigned int pTicks)
{
	Item::update(pTicks);
}

void Duplicator::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{
	if (mUsed)
	{
		return;
	}

	PlayerAvatar* lPlayerAvatar = (PlayerAvatar*)pAvatar;
	Rectangle lAbsPlayerHitBox = lPlayerAvatar->getAbsolutHitBox(lPlayerAvatar->getPosition());
	if (getAbsolutHitBox().testHit(lAbsPlayerHitBox))
	{
		PlayerAvatar* lNewAvatar = new PlayerAvatar();
		lNewAvatar->setLevel(mLevel);
		lNewAvatar->setPosition(mNewAvatarPosition);
		lNewAvatar->init();
		mLevel->addItem(lNewAvatar);
		mUsed = true;
	}
}

std::string Duplicator::typeName()
{
	return "Duplicator";
}

Item* Duplicator::createItem()
{
	return new Duplicator();
}
