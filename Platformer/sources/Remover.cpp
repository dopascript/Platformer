#include "Remover.h"
#include "SoundPlayer.h"
#include "Level.h"
#include "PlayerAvatar.h"

using namespace Platformer;

void Remover::init()
{
	setHitBox(Rectangle(16, 16, 0, -1));
	mSpeed = FPoint(0.0f, 0.0f);
}

void Remover::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));
}

void Remover::update(unsigned int pTicks)
{

}

void Remover::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{
	PlayerAvatar* lPlayerAvatar = (PlayerAvatar*)pAvatar;
	Rectangle lAbsPlayerHitBox = lPlayerAvatar->getAbsolutHitBox(lPlayerAvatar->getPosition());
	if (getAbsolutHitBox().testHit(lAbsPlayerHitBox) && 
		mLevel->getPlayerAvatars()->size() > 1)
	{
		pAvatar->startRemoveAnimation(pTime);
	}
}

std::string Remover::typeName()
{
	return "Remover";
}

Item* Remover::createItem()
{
	return new Remover();
}
