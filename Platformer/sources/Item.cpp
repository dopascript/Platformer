#include "Item.h"

#include <algorithm>

#include "Map.h"
#include "Level.h"
#include "LoadHelper.h"
#include "Area.h"

using namespace Platformer;

std::map<std::string, Item*> Item::mItemsTypes;

Item::Item()
{
	mItemsProximityCount = 0;
	mActive = false;
	mIsSolid = false;
	mRemoveAnimation = false;
	mNeedListItemsProximity = false;
	mMobile = false;
	mDrawColor = Color(255, 255, 255, 255);
}

void Item::setLevel(Level *pLevel)
{
	mLevel = pLevel;
}

void Item::setSprite(Sprite &pSprite)
{
	mSprite = pSprite;
}

Sprite* Item::getSprite()
{
	return &mSprite;
}

void Item::setPosition(Point pPosition)
{
	mPosition = pPosition;
}

Point Item::getPosition()
{
	return mPosition;
}

void Item::setHitBox(Rectangle pHitBox)
{
	mHitBox = pHitBox;
}

Rectangle Item::getHitBox()
{
	return mHitBox;
}

void Item::setSpeed(FPoint pSpeed)
{
	mSpeed = pSpeed;
}

FPoint Item::getSpeed()
{
	return mSpeed;
}

void Item::setIsSolid(bool pIsSolid)
{
	mIsSolid = pIsSolid;
}

bool Item::getIsSolid()
{
	return mIsSolid;
}

void Item::active()
{
	mActive = true;
}

bool Item::getActive()
{
	return mActive;
}

void Item::move(FPoint pDirection, CollisionTest pCollisionTest)
{
	int lNewX = mPosition.x;
	int lNewY = mPosition.y;

	if (pCollisionTest != CollisionTest_None)
	{
		int lOrientationX = pDirection.x < 0.0f ? -1 : 1;
		int lOrientationY = pDirection.y < 0.0f ? -1 : 1;

		int lMoveIterationCountX = std::abs((int)pDirection.x);
		int lMoveIterationCountY = std::abs((int)pDirection.y);

		int lMaxMove = std::max(lMoveIterationCountX, lMoveIterationCountY);

		bool lCollisionX = false;
		bool lCollisionY = false;

		for (int lIteration = 0; lIteration < lMaxMove; lIteration++)
		{
			if (lMoveIterationCountX > 0)
			{
				Point lNextPosition(lNewX + lOrientationX, lNewY);
				if (testHit(lNextPosition, pDirection, pCollisionTest))
				{
					if (!testHit(Point(lNewX + lOrientationX, lNewY - 1), pDirection, pCollisionTest))
					{
						lNewX += lOrientationX;
						lMoveIterationCountX--;
						lNewY--;
						lCollisionX = false;
					}
					else
					{
						lCollisionX = true;
					}
				}
				else
				{
					lNewX += lOrientationX;
					lMoveIterationCountX--;
					lCollisionX = false;
				}
			}
			if (lMoveIterationCountY > 0)
			{
				Point lNextPosition(lNewX, lNewY + lOrientationY);
				if (!testHit(lNextPosition, pDirection, pCollisionTest))
				{
					lNewY += lOrientationY;
					lMoveIterationCountY--;
					lCollisionX = false;
				}
				else
				{
					lCollisionY = true;
				}
			}
		}
		if (lCollisionX) mSpeed.x = 0.0f;
		if (lCollisionY) mSpeed.y = 0.0f;
	}
	else
	{
		lNewX = mPosition.x + (int)pDirection.x;
		lNewY = mPosition.y + (int)pDirection.y;
	}

	mPosition.x = lNewX;
	mPosition.y = lNewY;
}

bool Item::testHit(Point pPosition, FPoint pDirection, CollisionTest pCollisionTest)
{
	switch (pCollisionTest)
	{
	case CollisionTest_None:
	return false;
	case CollisionTest_MapAndItems:
		return testHitMap(pPosition, pDirection) || testHitSolidItems(pPosition);
	case CollisionTest_Map:
		return testHitMap(pPosition, pDirection);
	case CollisionTest_Items:
	return testHitSolidItems(pPosition);
	default:
		return false;
	}
}

Rectangle Item::getAbsolutHitBox()
{
	return getAbsolutHitBox(mPosition);
}

Rectangle Item::getAbsolutHitBox(Point pPosition)
{
	int lX = mHitBox.x + pPosition.x;
	int lY = mHitBox.y + pPosition.y;
	return Rectangle(mHitBox.width, mHitBox.height, lX, lY);
}

bool Item::testHitMap(Point pPosition, FPoint pDirection)
{
	Rectangle lAbsHitBox = getAbsolutHitBox(pPosition);
	Map* lCollisionMap = mLevel->getCollisionMap();
	if (lAbsHitBox.x < 0 || lAbsHitBox.y < 0 || lAbsHitBox.x + lAbsHitBox.width >= lCollisionMap->getSize().width * lCollisionMap->getTileSize())
	{
		return true;
	}
	return lCollisionMap->testHit(lAbsHitBox, pDirection);
}

std::vector<Item*> Item::findHitItems(Point pPosition)
{
	Rectangle lAbsHitBox = getAbsolutHitBox(pPosition);
	std::vector<Item*> lResult;
	//for (auto lItem : *mLevel->getItems())
	for (int i = 0; i < mItemsProximityCount; i++)
	{
		Item* lItem = mItemsProximity[i];
		if (lItem == this) continue;

		if (lAbsHitBox.testHit(lItem->getAbsolutHitBox()))
		{
			lResult.push_back(lItem);
		}
	}
	return lResult;
}

bool Item::testHitSolidItems(Point pPosition)
{
	Rectangle lAbsHitBox = getAbsolutHitBox(pPosition);
	std::vector<Item*> lResult;
	//for (auto lItem : *mLevel->getItems())
	for(int i = 0;i < mItemsProximityCount;i++)
	{
		Item* lItem = mItemsProximity[i];
		if (lItem == this || !lItem->getIsSolid()) continue;

		if (lAbsHitBox.testHit(lItem->getAbsolutHitBox()))
		{
			return true;
		}
	}
	return false;
}

void Item::addItemProximity(Item* pItem)
{
	if (!mNeedListItemsProximity || mItemsProximityCount == ITEMS_PROXIMITY_MAX - 1)
	{
		return;
	}

	if (mItemsProximityCount > 0)
	{
		for (int i = mItemsProximityCount - 1; i >= 0; i--)
		{
			if (mItemsProximity[i] == pItem)
			{
				return;
			}
		}
	}

	mItemsProximity[mItemsProximityCount] = pItem;
	mItemsProximityCount++;
}

void Item::clearItemsProximity()
{
	mItemsProximityCount = 0;
}

void Item::update(unsigned int pTicks)
{
	if (mMobile)
	{
		for (int lItemProximityIndex = 0; lItemProximityIndex < mItemsProximityCount; lItemProximityIndex++)
		{
			Item* lItemProximity = mItemsProximity[lItemProximityIndex];
			lItemProximity->onMobileItemProximity(pTicks, this);
		}
	}
}

void Item::updateRemoveAnimation(unsigned int pTicks)
{
	int pTimeDiff = pTicks - mRemoveAnimationStartTime;
	if (pTimeDiff > 500)
	{
		mLevel->removeItem(this);
	}
	else
	{
		mDrawColor.a = 255 - (int)((((float)pTimeDiff) / 500.0f) * 254);
	}
}

void Item::draw(SDL_Renderer *pSDL_Renderer, Point pCameraShift, int pTime)
{
	SDL_Rect lTextureRectangle = mSprite.getTextureRectangle(pTime).toSDL_Rect();
	SDL_Rect lOnScreenRectangle = mSprite.getOnScreenRectangle(mPosition).toSDL_Rect();
	lOnScreenRectangle.x += pCameraShift.x;
	lOnScreenRectangle.y += pCameraShift.y;

	SDL_Point lCenter = mSprite.getOrigin().toSDL_Point();
	SDL_RendererFlip lHorizontalFlip = mSprite.getHorizontalFlip() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	SDL_SetTextureColorMod(mSprite.getTexture(), mDrawColor.r, mDrawColor.g, mDrawColor.b);
	SDL_SetTextureAlphaMod(mSprite.getTexture(), mDrawColor.a);
	SDL_RenderCopyEx(
		pSDL_Renderer,
		mSprite.getTexture(),
		&lTextureRectangle,
		&lOnScreenRectangle,
		0,
		&lCenter,
		lHorizontalFlip);
}

void Item::updateOnScreenValue(Rectangle &mScreenHitBox)
{
	mIsOnScreen = mSprite.getOnScreenRectangle(mPosition).testHit(mScreenHitBox);
}

bool Item::getIsOnScreen()
{
	return mIsOnScreen;
}

void Item::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{

}

void Item::onMobileItemProximity(unsigned int pTime, Item* pItem)
{

}

void Item::startRemoveAnimation(unsigned int pTime)
{
	mRemoveAnimation = true;
	mRemoveAnimationStartTime = pTime;
}