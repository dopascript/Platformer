#include "BlocBonus.h"
#include "ImageLibrary.h"
#include "SoundPlayer.h"
#include "Level.h"
#include "BonusMushroom.h"

using namespace Platformer;

void BlocBonus::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/map_decor.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(16, 16));
	lPlayerSprite.setOrigin(Point(0, 0));
	lPlayerSprite.setSpriteCountY(29);

	setSprite(lPlayerSprite);


	SpriteAnimation lAnimation("bloc");
	lAnimation.addFrame(580);
	lAnimation.addFrame(580);
	lAnimation.addFrame(580);
	lAnimation.addFrame(581);
	lAnimation.addFrame(581);
	lAnimation.addFrame(581);
	lAnimation.addFrame(582);
	lAnimation.addFrame(582);
	lAnimation.addFrame(582);
	lAnimation.addFrame(583);
	lAnimation.addFrame(583);
	lAnimation.addFrame(583);
	lPlayerSprite.addAnimation(lAnimation);
	setSprite(lPlayerSprite);

	mSprite.playAnimation("bloc");

	setHitBox(Rectangle(16, 16, 0, -1));

	mSpeed = FPoint(0.0f, 0.0f);
	mIsSolid = true;
	mInitialPosition = mPosition;

	SoundPlayer::getInstance()->addSound("vine", "sounds/vine.wav");
}

void BlocBonus::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));
	mInitialPosition = mPosition;
}

void BlocBonus::update(unsigned int pTicks)
{
	mPosition.y = mPosition.y + (int)mSpeed.y;
	float lSpeedY = mSpeed.y + mAcceleration.y;
	int lPositionDifY = mPosition.y - mInitialPosition.y;

	if (std::abs(mPosition.y - mInitialPosition.y) <= 1)
	{
		mSpeed.y = 0.0f;
		mPosition.y = mInitialPosition.y;
	}
	else
	{
		mSpeed.y = lSpeedY;
	}

	Item::update(pTicks);
}

void BlocBonus::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{
	PlayerAvatar* lPlayerAvatar = (PlayerAvatar*)pAvatar;
	Point lPlayerPosition = lPlayerAvatar->getPosition();

	Point lPlayerTopPosition(lPlayerPosition.x, lPlayerPosition.y - 1);
	Rectangle lAbsPlayerTopHitBox = lPlayerAvatar->getAbsolutHitBox(lPlayerTopPosition);
	if (getAbsolutHitBox().testHit(lAbsPlayerTopHitBox))
	{
		createBonus(pTime);
		mSpeed.y = -3.0f;
		mAcceleration = FPoint(0.0f, 0.7f);

		FPoint lPlayerSpeed = lPlayerAvatar->getSpeed();
		lPlayerSpeed.y = 0;
		lPlayerAvatar->setSpeed(lPlayerSpeed);

		SoundPlayer::getInstance()->playSound("vine");	
	}
}

void BlocBonus::createBonus(unsigned int pTicks)
{
	BonusMushroom* bonus = new BonusMushroom();
	bonus->setBlocOrigine(this);
	bonus->setLevel(mLevel);
	bonus->init();

	int lDecalX = (mHitBox.width / 2) - (bonus->getHitBox().width / 2);
	int lDecalY = (mHitBox.height / 2) - (bonus->getHitBox().height / 2);

	bonus->setPosition(Point(mPosition.x + lDecalX, mPosition.y + lDecalY - 3));

	mLevel->addItemToBack(bonus);
}

std::string BlocBonus::typeName()
{
	return "BlocBonus";
}

Item* BlocBonus::createItem()
{
	return new BlocBonus();
}
