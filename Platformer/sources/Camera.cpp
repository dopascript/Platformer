#include "Camera.h"
#include <algorithm>

using namespace Platformer;

void Camera::setItemToTrack(Item* pItemToTrack)
{
	mItemToTrack = pItemToTrack;
}

void Camera::setScreenSize(Size pSize)
{
	mScreenSize = pSize;
}

void Camera::setCollistionMap(Map* pMap)
{
	mCollisionMap = pMap;
}

void Camera::init()
{

}

void Camera::update(unsigned int pTicks)
{
	int lCameraShiftX = mItemToTrack->getPosition().x - (mScreenSize.width / 2);
	int lCameraShiftY = mItemToTrack->getPosition().y - (mScreenSize.width / 2);

	int lPixelMapWidth = mCollisionMap->getSize().width * mCollisionMap->getTileSize();
	if (lPixelMapWidth < mScreenSize.width)
	{
		lCameraShiftX = ((mScreenSize.width - lPixelMapWidth) / 2);
	}
	else
	{
		lCameraShiftX = std::min(std::max(lCameraShiftX, 0), lPixelMapWidth - mScreenSize.width);
	}

	int lPixelMapHeight = mCollisionMap->getSize().height * mCollisionMap->getTileSize();
	if (lPixelMapHeight < mScreenSize.height)
	{
		lCameraShiftY = -(mScreenSize.height - lPixelMapHeight) / 2;
	}
	else
	{
		lCameraShiftY = std::min(std::max(lCameraShiftY, 0), lPixelMapHeight - mScreenSize.height);
	}
	mCurrentShift.x = lCameraShiftX * -1;
	mCurrentShift.y = lCameraShiftY * -1;
}

void Camera::uninit()
{

}

Point Camera::getShift()
{
	return mCurrentShift;
}
