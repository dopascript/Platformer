#include "BonusMushroom.h"

#include <algorithm>

#include "Input.h"
#include "ImageLibrary.h"
#include "Level.h"
#include "ItemHelper.h"
#include "SoundPlayer.h"

using namespace Platformer;

BonusMushroom::BonusMushroom()
{
	mBlocOrigine = nullptr;
}

void BonusMushroom::setBlocOrigine(Item *pBlocOrigine)
{
	mBlocOrigine = pBlocOrigine;
}

void BonusMushroom::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/mushroom.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(16, 16));
	lPlayerSprite.setOrigin(Point(0, 0));

	setSprite(lPlayerSprite);

	mSprite.setFrame(3);

	setHitBox(Rectangle(16, 16, 0, -1));

	mSpeed = FPoint(2.1f, 0);

	SoundPlayer::getInstance()->addSound("power-up", "sounds/power-up.wav");

	mNeedListItemsProximity = true;
}

void BonusMushroom::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));
}

void BonusMushroom::update(unsigned int pTicks)
{
	if (!mIsOnScreen)
	{
		mLevel->removeItem(this);
		return;
	}

	for (auto &lPlayerAvatar : *mLevel->getPlayerAvatars())
	{
		Rectangle lHitbox = lPlayerAvatar->getAbsolutHitBox();
		if (getAbsolutHitBox().testHit(lHitbox))
		{
			SoundPlayer::getInstance()->playSound("power-up");
			lPlayerAvatar->setForm(PlayerAvatarState_Raccoon);
			mLevel->removeItem(this);
			return;
		}
	}

	if (mBlocOrigine != nullptr &&
		getAbsolutHitBox().testHit(mBlocOrigine->getAbsolutHitBox()))
	{
		mPosition.y = mPosition.y - 1;
		mPosition.y = std::min(mPosition.y, mBlocOrigine->getPosition().y - 3);
	}
	else
	{
		ItemClassicMove(mLevel, this);
	}
}

std::string BonusMushroom::typeName()
{
	return "BonusMushroom";
}

Item* BonusMushroom::createItem()
{
	return new BonusMushroom();
}