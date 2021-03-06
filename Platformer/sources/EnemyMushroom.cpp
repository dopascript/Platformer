#include "EnemyMushroom.h"

#include <algorithm>

#include "Input.h"
#include "ImageLibrary.h"
#include "Level.h"
#include "ItemHelper.h"
#include "SoundPlayer.h"

using namespace Platformer;

void EnemyMushroom::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/mushroom.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(16, 16));
	lPlayerSprite.setOrigin(Point(8, 16));

	SpriteAnimation lAnimation("run");
	lAnimation.addFrame(0);
	lAnimation.addFrame(0);
	lAnimation.addFrame(0);
	lAnimation.addFrame(0);
	lAnimation.addFrame(1);
	lAnimation.addFrame(1);
	lAnimation.addFrame(1);
	lAnimation.addFrame(1);
	lPlayerSprite.addAnimation(lAnimation);

	setSprite(lPlayerSprite);

	mSprite.playAnimation("run");

	setHitBox(Rectangle(16, 16, -8, -17));

	mSpeed = FPoint(-2.1f,0);
	mIsDead = false;
	mMobile = true;
	SoundPlayer::getInstance()->addSound("stomp", "sounds/stomp.wav");
}

void EnemyMushroom::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));
}

void EnemyMushroom::update(unsigned int pTicks)
{
	Map* lCollisionMap = mLevel->getCollisionMap();
	if (mPosition.y > lCollisionMap->getTileSize() * lCollisionMap->getSize().height)
	{
		mLevel->removeItem(this);
		return;
	}

	if (mIsDead)
	{
		if (pTicks - mKillTime > 1000)
		{
			mLevel->removeItem(this);
		}
	}
	else
	{
		Map* lCollisionMap = mLevel->getCollisionMap();
		int lDirection = mSpeed.x > 0.0f ? 1 : -1;
		Point lNextBlocPosition(mPosition.x + (lDirection * lCollisionMap->getTileSize()), mPosition.y + lCollisionMap->getTileSize() + 1);
		if (!mLevel->getCollisionMap()->testHit(getAbsolutHitBox(lNextBlocPosition), FPoint(0.0f, 0.0f)))
		{
			mSpeed.x *= -1;
		}

		ItemClassicMove(mLevel, this);
	}

	Item::update(pTicks);
}

void EnemyMushroom::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{
	if (mIsDead)
	{
		return;
	}

	PlayerAvatar* lPlayerAvatar = (PlayerAvatar*)pAvatar;
	FPoint lPlayerSpeed = lPlayerAvatar->getSpeed();
	if (lPlayerAvatar->getSpeed().y > 1.0f)
	{
		Point lBottomPosition(mPosition.x, mPosition.y + 1);
		Rectangle lAbsBottomHitBox = getAbsolutHitBox(lBottomPosition);
		if (getAbsolutHitBox().testHit(lPlayerAvatar->getAbsolutHitBox()))
		{
			hit(pTime);
			lPlayerSpeed.y = -5.0f;
			lPlayerAvatar->setSpeed(lPlayerSpeed);
			lPlayerAvatar->setKilling(pTime);
			SoundPlayer::getInstance()->playSound("stomp");
			return;
		}
	}

	if (!lPlayerAvatar->isKilling(pTime))
	{
		Rectangle lHitbox = lPlayerAvatar->getAbsolutHitBox();
		if (getAbsolutHitBox().testHit(lHitbox))
		{
			lPlayerAvatar->hit(pTime);
			return;
		}
	}
}

void EnemyMushroom::hit(unsigned int pTicks)
{
	mIsDead = true;
	mSpeed = FPoint(0, 0);
	mKillTime = pTicks;
	mSprite.setFrame(2);
}

std::string EnemyMushroom::typeName()
{
	return "EnemyMushroom";
}

Item* EnemyMushroom::createItem()
{
	return new EnemyMushroom();
}