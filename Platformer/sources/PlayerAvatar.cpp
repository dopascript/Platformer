#include "PlayerAvatar.h"

#include <algorithm>

#include "Input.h"
#include "ImageLibrary.h"
#include "Level.h"
#include "SoundPlayer.h"

using namespace Platformer;

void PlayerAvatar::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/mario.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(32, 32));
	lPlayerSprite.setSpriteCountY(10);
	lPlayerSprite.setOrigin(Point(16, 31));

	SpriteAnimation lAnimation_run("run");
	lAnimation_run.addFrame(30);
	lAnimation_run.addFrame(30);
	lAnimation_run.addFrame(30);
	lAnimation_run.addFrame(30);
	lAnimation_run.addFrame(31);
	lAnimation_run.addFrame(31);
	lAnimation_run.addFrame(31);
	lAnimation_run.addFrame(31);
	lAnimation_run.addFrame(32);
	lAnimation_run.addFrame(32);
	lAnimation_run.addFrame(32);
	lAnimation_run.addFrame(32);
	lPlayerSprite.addAnimation(lAnimation_run);

	SpriteAnimation lAnimation_tinyrun("run_tiny");
	lAnimation_tinyrun.addFrame(0);
	lAnimation_tinyrun.addFrame(0);
	lAnimation_tinyrun.addFrame(0);
	lAnimation_tinyrun.addFrame(0);
	lAnimation_tinyrun.addFrame(1);
	lAnimation_tinyrun.addFrame(1);
	lAnimation_tinyrun.addFrame(1);
	lAnimation_tinyrun.addFrame(1);
	lPlayerSprite.addAnimation(lAnimation_tinyrun);

	SpriteAnimation lAnimation_raccoonrun("run_raccoon");
	lAnimation_raccoonrun.addFrame(90);
	lAnimation_raccoonrun.addFrame(90);
	lAnimation_raccoonrun.addFrame(90);
	lAnimation_raccoonrun.addFrame(90);
	lAnimation_raccoonrun.addFrame(91);
	lAnimation_raccoonrun.addFrame(91);
	lAnimation_raccoonrun.addFrame(91);
	lAnimation_raccoonrun.addFrame(91);
	lPlayerSprite.addAnimation(lAnimation_raccoonrun);

	SpriteAnimation lAnimation_raccoonflight("flight_raccoon");
	lAnimation_raccoonflight.addFrame(110);
	lAnimation_raccoonflight.addFrame(110);
	lAnimation_raccoonflight.addFrame(110);
	lAnimation_raccoonflight.addFrame(110);
	lAnimation_raccoonflight.addFrame(111);
	lAnimation_raccoonflight.addFrame(111);
	lAnimation_raccoonflight.addFrame(111);
	lAnimation_raccoonflight.addFrame(111);
	lPlayerSprite.addAnimation(lAnimation_raccoonflight);


	setSprite(lPlayerSprite);

	mIsUntouchable = false;
	mUntouchableRemainingTime = 0;
	mUsingParachute = false;

	setForm(PlayerAvatarState_Tiny);

	SoundPlayer::getInstance()->addSound("jump", "sounds\\jump.wav");
	SoundPlayer::getInstance()->addSound("player-down", "sounds\\player_down.wav");
}

void PlayerAvatar::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));
}

void PlayerAvatar::update(unsigned int pTicks)
{
	if (!mIsDead)
	{
		updateSpecialAppearance(pTicks);
		updateMoves(pTicks);

		Map* lCollisionMap = mLevel->getCollisionMap();
		if (mPosition.y - mHitBox.height > lCollisionMap->getTileSize() * lCollisionMap->getSize().height)
		{
			kill(pTicks);
		}
	}
	else
	{
		float lSpeedY = mSpeed.y + 0.2f;
		mSpeed.y = std::min(lSpeedY, 6.0f);
		move(mSpeed, CollisionTest_None);
	}
}

void PlayerAvatar::hit(unsigned int pTicks)
{
	if (mIsUntouchable)
	{
		return;
	}

	switch (mPlayerAvatarForm)
	{
	case PlayerAvatarState_Tiny:
		kill(pTicks);
		break;
	case PlayerAvatarState_Big:
		setForm(PlayerAvatarState_Tiny);
		mIsUntouchable = true;
		mUntouchableStartTime = pTicks;
		mUntouchableRemainingTime = 2000;
		break;
	case PlayerAvatarState_Raccoon:
		setForm(PlayerAvatarState_Big);
		mIsUntouchable = true;
		mUntouchableStartTime = pTicks;
		mUntouchableRemainingTime = 2000;
		break;
	case CollisionTest_Superball:
		break;
	}
}

void PlayerAvatar::updateSpecialAppearance(unsigned int pTicks)
{
	if (mIsUntouchable)
	{
		if ((int)pTicks - mUntouchableStartTime > mUntouchableRemainingTime)
		{
			mIsUntouchable = false;
			mDrawColor = Color(255, 255, 255, 255);
		}
		else
		{
			mDrawColor = Color(255, 255 * (((pTicks - mUntouchableStartTime) / 100) % 2), 255, 255);
		}
	}
}

void PlayerAvatar::updateMoves(unsigned int pTicks)
{
	Point lBottomPosition(mPosition.x, mPosition.y + 1);
	bool lIsOnSolid = testHit(lBottomPosition, mSpeed, CollisionTest_MapAndItems);

	//Horizontal move
	float lSpeedX = mSpeed.x;

	if (Input::getInstance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		lSpeedX -= lIsOnSolid ? 0.8f : 0.35f;
		mSprite.setHorizontalFlip(false);
		mSprite.playAnimation(mRunAnimation);
	}
	else if (Input::getInstance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		lSpeedX += lIsOnSolid ? 0.8f : 0.35f;
		mSprite.setHorizontalFlip(true);
		mSprite.playAnimation(mRunAnimation);
	}
	else if (lIsOnSolid)
	{
		if (lSpeedX < -1.0f) lSpeedX += 0.9f;
		else if (lSpeedX > 1.0f) lSpeedX -= 0.9f;
		else lSpeedX = 0.0f;
	}
	lSpeedX = std::min(std::max(lSpeedX, -6.0f), 6.0f);

	if (std::abs(lSpeedX) < 0.01f)
	{
		mSprite.setFrame(mWaitFrame);
	}

	//Vertical move
	float lSpeedY = mSpeed.y;
	if (lIsOnSolid)
	{
		if (Input::getInstance()->isPress(SDL_SCANCODE_SPACE))
		{
			lSpeedY = -13.0f;
			SoundPlayer::getInstance()->playSound("jump");
		}
		else
		{
			lSpeedY = 8;
		}
		mUsingParachute = false;
	}
	else
	{
		if (mPlayerAvatarForm == PlayerAvatarState_Raccoon &&
			Input::getInstance()->isKeyDown(SDL_SCANCODE_SPACE) &&
			lSpeedY > 0.0f)
		{
 			lSpeedY = 1.0f;
			
			mSprite.playAnimation("flight_raccoon");
			mUsingParachute = true;
		}
		else
		{
			lSpeedY += 1.0f;
			lSpeedY = std::min(lSpeedY, 9.0f);

			mSprite.setFrame(mJumpFrame);
			mUsingParachute = false;
		}
	}

	mSpeed.x = lSpeedX;
	mSpeed.y = lSpeedY;

	//Move
	move(mSpeed, CollisionTest_MapAndItems);
}

void PlayerAvatar::kill(unsigned int pTicks)
{
	if (mIsDead) return;

	mIsDead = true;
	mSprite.setFrame(20);
	mSpeed = FPoint(0, -5);
	SoundPlayer::getInstance()->playSound("player-down");
}

bool PlayerAvatar::isDead()
{
	return mIsDead;
}

void PlayerAvatar::setForm(PlayerAvatarForm pPlayerAvatarForm)
{
	mPlayerAvatarForm = pPlayerAvatarForm;
	switch (mPlayerAvatarForm)
	{
	case PlayerAvatarState_Tiny:
		mJumpFrame = 3;
		mWaitFrame = 0;
		mRunAnimation = "run_tiny";
		setHitBox(Rectangle(13, 15, -6, -15));
	break;
	case PlayerAvatarState_Big:
		mJumpFrame = 33;
		mWaitFrame = 30;
		mRunAnimation = "run";

		setHitBox(Rectangle(13, 26, -6, -26));
	break;
	case PlayerAvatarState_Raccoon:
		mJumpFrame = 93;
		mWaitFrame = 90;
		mRunAnimation = "run_raccoon";

		setHitBox(Rectangle(13, 26, -6, -26));
	break;
	case CollisionTest_Superball:
	break;
	default:
	break;
	}
}

std::string PlayerAvatar::typeName()
{
	return "PlayerAvatar";
}

Item* PlayerAvatar::createItem()
{
	return new PlayerAvatar();
}