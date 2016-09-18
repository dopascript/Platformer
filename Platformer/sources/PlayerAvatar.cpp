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

	SpriteAnimation lAnimation_jetpackrun("run_jetpack");
	lAnimation_jetpackrun.addFrame(90);
	lAnimation_jetpackrun.addFrame(90);
	lAnimation_jetpackrun.addFrame(90);
	lAnimation_jetpackrun.addFrame(90);
	lAnimation_jetpackrun.addFrame(91);
	lAnimation_jetpackrun.addFrame(91);
	lAnimation_jetpackrun.addFrame(91);
	lAnimation_jetpackrun.addFrame(91);
	lPlayerSprite.addAnimation(lAnimation_jetpackrun);

	SpriteAnimation lAnimation_jetpackflight("flight_jetpack");
	lAnimation_jetpackflight.addFrame(110);
	lAnimation_jetpackflight.addFrame(110);
	lAnimation_jetpackflight.addFrame(110);
	lAnimation_jetpackflight.addFrame(110);
	lAnimation_jetpackflight.addFrame(111);
	lAnimation_jetpackflight.addFrame(111);
	lAnimation_jetpackflight.addFrame(111);
	lAnimation_jetpackflight.addFrame(111);
	lPlayerSprite.addAnimation(lAnimation_jetpackflight);


	setSprite(lPlayerSprite);

	mUntouchable = false;
	mUntouchableRemainingTime = 0;
	mUsingParachute = false;
	mNeedListItemsProximity = true;
	mKillingTime = 0;
	mDeathTime = 0;

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
	if (mRemoveAnimation)
	{
		updateRemoveAnimation(pTicks);
	}
	else if(mIsDead)
	{
		float lSpeedY = mSpeed.y + 0.2f;
		mSpeed.y = std::min(lSpeedY, 6.0f);
		move(mSpeed, CollisionTest_None);
		if (pTicks - mDeathTime > 2000)
		{
			mLevel->setLevelToLoad("levels/level1.json", Point(-1, -1));
		}
	}
	else
	{
		updateSpecialAppearance(pTicks);
		updateMoves(pTicks);

		Map* lCollisionMap = mLevel->getCollisionMap();
		if (mPosition.y - mHitBox.height > lCollisionMap->getTileSize() * lCollisionMap->getSize().height)
		{
			kill(pTicks);
		}

		for (int lItemProximityIndex = 0; lItemProximityIndex < mItemsProximityCount; lItemProximityIndex++)
		{
			Item* lItemProximity = mItemsProximity[lItemProximityIndex];
			lItemProximity->onAvatarProximity(pTicks, this);
		}
	}
	
}

void PlayerAvatar::hit(unsigned int pTicks)
{
	if (mUntouchable)
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
		mUntouchable = true;
		mUntouchableStartTime = pTicks;
		mUntouchableRemainingTime = 2000;
		break;
	case PlayerAvatarState_Jetpack:
		setForm(PlayerAvatarState_Big);
		mUntouchable = true;
		mUntouchableStartTime = pTicks;
		mUntouchableRemainingTime = 2000;
		break;
	case CollisionTest_Superball:
		break;
	}
}

void PlayerAvatar::updateSpecialAppearance(unsigned int pTicks)
{
	if (mUntouchable)
	{
		if ((int)pTicks - mUntouchableStartTime > mUntouchableRemainingTime)
		{
			mUntouchable = false;
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

	bool lHitOnBottom = testHit(lBottomPosition, mSpeed, CollisionTest_MapAndItems);

	bool lHitOnLeft = testHit(Point(mPosition.x - 1, mPosition.y), mSpeed, CollisionTest_MapAndItems);
	bool lHitOnRight = testHit(Point(mPosition.x + 1, mPosition.y), mSpeed, CollisionTest_MapAndItems);

	int lTileSize = mLevel->getCollisionMap()->getTileSize();
	Point lTilePosition(mPosition.x / lTileSize, mPosition.y / lTileSize);

	TileCollision lTileCollision = mLevel->getCollisionMap()->getTileCollision(lTilePosition);
	if (lTileCollision == TileCollision::TileCollision_None)
	{
		Point lBottomTileCollision = Point(lTilePosition.x, lTilePosition.y + 1);
		lTileCollision = mLevel->getCollisionMap()->getTileCollision(lBottomTileCollision);
	}
	
	//Horizontal move
	float lSpeedX = mSpeed.x;
	mCrouched = false;
	mSlidding = false;
	bool lRunning = Input::getInstance()->isKeyDown(SDL_SCANCODE_LCTRL);
	if (lHitOnBottom)
	{	
		bool lWantToMove = false;
		if (mTracked && Input::getInstance()->isKeyDown(SDL_SCANCODE_DOWN) && mTracked)
		{
			switch (lTileCollision)
			{
			case TileCollision_Angle45:
				lSpeedX -= 1.0f;
				lWantToMove = true;
				mSlidding = true;
				mSprite.setFrame(mSliddingFrame);
				break;
			case TileCollision_Angle22_1:
			case TileCollision_Angle22_2:
				lSpeedX -= 0.2f;
				lWantToMove = true;
				mSlidding = true;
				mSprite.setFrame(mSliddingFrame);
				break;
			case TileCollision_Angle135:
				lSpeedX += 1.0f;
				lWantToMove = true;
				mSlidding = true;
				mSprite.setFrame(mSliddingFrame);
				break;
			case TileCollision_Angle112_1:
			case TileCollision_Angle112_2:
				lSpeedX += 0.2f;
				lWantToMove = true;
				mSlidding = true;
				mSprite.setFrame(mSliddingFrame);
				break;
			default:
				mCrouched = true;
				mSprite.setFrame(mCrouchedFrame);
				break;
			}
			
		}
		else if (mTracked && Input::getInstance()->isKeyDown(SDL_SCANCODE_LEFT) && mTracked)
		{
			if (lRunning)
			{
				lSpeedX -= 0.8f;
			}
			else
			{
				lSpeedX -= 0.4f;
			}
			
			mSprite.setHorizontalFlip(false);
			mSprite.playAnimation(mRunAnimation);
			lWantToMove = true;
		}
		else if (mTracked && Input::getInstance()->isKeyDown(SDL_SCANCODE_RIGHT) && mTracked)
		{
			if (lRunning)
			{
				lSpeedX += 0.8f;
			}
			else
			{
				lSpeedX += 0.4f;
			}
			mSprite.setHorizontalFlip(true);
			mSprite.playAnimation(mRunAnimation);
			lWantToMove = true;
		}

		if(!lWantToMove)
		{
			if (lSpeedX < -1.0f) lSpeedX += 0.9f;
			else if (lSpeedX > 1.0f) lSpeedX -= 0.9f;
			else lSpeedX = 0.0f;
		}
	}
	else if(mTracked)
	{
		if (Input::getInstance()->isKeyDown(SDL_SCANCODE_LEFT))
		{
			lSpeedX -= lHitOnBottom ? 0.8f : 0.35f;
			mSprite.setHorizontalFlip(false);
			mSprite.playAnimation(mRunAnimation);
		}
		else if (Input::getInstance()->isKeyDown(SDL_SCANCODE_RIGHT))
		{
			lSpeedX += lHitOnBottom ? 0.8f : 0.35f;
			mSprite.setHorizontalFlip(true);
			mSprite.playAnimation(mRunAnimation);
		} 
	}
	if (lRunning)
	{
		lSpeedX = std::min(std::max(lSpeedX, -7.0f), 7.0f);
	}
	else
	{
		lSpeedX = std::min(std::max(lSpeedX, -4.0f), 4.0f);
	}
	

	if (std::abs(lSpeedX) < 0.01f && !mCrouched)
	{
		mSprite.setFrame(mWaitFrame);
	}

	//Vertical move
	float lSpeedY = mSpeed.y;
	if (lHitOnBottom)
	{
		if (mTracked && Input::getInstance()->isPress(SDL_SCANCODE_SPACE))
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
		if (mTracked && 
			mPlayerAvatarForm == PlayerAvatarState_Jetpack &&
			Input::getInstance()->isKeyDown(SDL_SCANCODE_SPACE) &&
			lSpeedY > 0.0f)
		{
 			lSpeedY = 1.0f;
			
			mSprite.playAnimation("flight_jetpack");
			mUsingParachute = true;
		}
		else
		{
			lSpeedY += 1.0f;
			if (mTracked && 
				(lHitOnLeft && Input::getInstance()->isKeyDown(SDL_SCANCODE_LEFT)) ||
				(lHitOnRight && Input::getInstance()->isKeyDown(SDL_SCANCODE_RIGHT)))
			{
				lSpeedY = std::min(lSpeedY, 3.0f);
			}
			else
			{
				lSpeedY = std::min(lSpeedY, 9.0f);
			}

			mSprite.setFrame(mJumpFrame);
			mUsingParachute = false;
		}
	}

	if (mTracked && !lHitOnBottom && Input::getInstance()->isPress(SDL_SCANCODE_SPACE))
	{
		if (lHitOnLeft)
		{
			lSpeedX = 8;
			lSpeedY = -10.0f;
		}
		else if (lHitOnRight)
		{
			lSpeedX = -8;
			lSpeedY = -10.0f;
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
	mDeathTime = pTicks;
	mSprite.setFrame(20);
	mSpeed = FPoint(0, -5);
	SoundPlayer::getInstance()->playSound("player-down");

	for (auto &lPlayerAvatar : *mLevel->getPlayerAvatars())
	{
		if (lPlayerAvatar != this && !lPlayerAvatar->isDead())
		{
			lPlayerAvatar->kill(pTicks);
		}
	}
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
		mCrouchedFrame = 0;
		mSliddingFrame = 7;
		mRunAnimation = "run_tiny";
		setHitBox(Rectangle(8, 15, -4, -15));
	break;
	case PlayerAvatarState_Big:
		mJumpFrame = 33;
		mWaitFrame = 30;
		mCrouchedFrame = 34;
		mSliddingFrame = 37;
		mRunAnimation = "run";

		setHitBox(Rectangle(8, 26, -4, -26));
	break;
	case PlayerAvatarState_Jetpack:
		mJumpFrame = 93;
		mWaitFrame = 90;
		mSliddingFrame = 97;
		mCrouchedFrame = 94;
		mRunAnimation = "run_jetpack";

		setHitBox(Rectangle(8, 26, -4, -26));
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

void PlayerAvatar::setKilling(unsigned int pTime)
{
	mKillingTime = pTime;
}

bool PlayerAvatar::isKilling(unsigned int pTime)
{
	return pTime - mKillingTime < 100;
}

void PlayerAvatar::setTracked(bool pTracked)
{
	mTracked = pTracked;
}

bool PlayerAvatar::getTracked()
{
	return mTracked;
}