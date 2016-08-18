#ifndef CAMERA_H
#define CAMERA_H

#include "Item.h"
#include "Point.h"
#include "Size.h"
#include "Map.h"

namespace Platformer
{
	class Camera
	{
	public:
		void setScreenSize(Size pSize);
		void setCollistionMap(Map* pMap);
		void setItemToTrack(Item* pItemToTrack);

		void init();
		void update(unsigned int pTicks);

		void uninit();

		Point getShift();
	protected:
		Map* mCollisionMap;
		Point mCurrentShift;
		Item* mItemToTrack;
		Size mScreenSize;
	};
}

#endif
