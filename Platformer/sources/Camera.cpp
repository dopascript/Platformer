#include "Camera.h"
#include "Level.h"
#include "PlayerAvatar.h"
#include <algorithm>

using namespace Platformer;

Camera::Camera()
{
	mItemToTrack = nullptr;
}

void Camera::setLevel(Level* pLevel)
{
	mLevel = pLevel;
}

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
	Point lPositionToTrack(0,0);
	if (mItemToTrack != nullptr)
	{
		lPositionToTrack = mItemToTrack->getPosition();
	}
	else
	{
		int lAvatarCount = 0;
		PlayerAvatar* lFirstAvatar = *(mLevel->getPlayerAvatars()->begin());
		for (PlayerAvatar* lAvatar : *(mLevel->getPlayerAvatars()))
		{
			if (lAvatarCount > 0 && 
				(std::abs(lFirstAvatar->getPosition().x - lAvatar->getPosition().x) > mScreenSize.width - 100 ||
				 std::abs(lFirstAvatar->getPosition().y - lAvatar->getPosition().y) > mScreenSize.height - 100))
			{
				continue;
			}
			lPositionToTrack.x += lAvatar->getPosition().x;
			lPositionToTrack.y += lAvatar->getPosition().y;
			lAvatarCount++;
		}
		lPositionToTrack.x /= lAvatarCount;
		lPositionToTrack.y /= lAvatarCount;
	}

	int lCameraShiftX = lPositionToTrack.x - (mScreenSize.width / 2);
	int lCameraShiftY = lPositionToTrack.y - (mScreenSize.width / 2);

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
