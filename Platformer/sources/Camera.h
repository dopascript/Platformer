#ifndef CAMERA_H
#define CAMERA_H

#include "Point.h"
#include "Size.h"

namespace Platformer
{
	class Item;
	class Map;
	class Level;

	class Camera
	{
	public:
		Camera();
		void setLevel(Level* pLevel);
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
		Level* mLevel;
	};
}

#endif
