#ifndef AREA_MANAGER_H
#define AREA_MANAGER_H

#include "Size.h"
#include <list>

#define AREA_SIZE 30

namespace Platformer
{
	class Item;
	class Area;

	class AreaManager
	{
	public:
		void setLevelSize(Size pLevelSize);
		void init();
		void uninit();
		void updateItems(std::list<Item*>* pItems);
		void addItem(Item* mItem);
	private:
		Size mLevelSize;
		int mAreaCountX;
		int mAreaCountY;
		Area* mAreas;
	};
}

#endif