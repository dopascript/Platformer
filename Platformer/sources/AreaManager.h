#ifndef AREAS_MANAGER_H
#define AREAS_MANAGER_H

#include <list>
#include "Item.h"
#include "MainMenu.h"

#define AREA_SIZE 100

namespace Platformer
{
	class Area;

	class AreaManager
	{
	public:
		void setLevelSize(Size pLevelSize);
		void init();
		void updateItems(std::list<Item*>* pItems);
		void updateItem(Item* pItem);
		void uninit();

		Area* getAreaAtPosition(Point pPosition);
		
	protected:
		Size mLevelSize;
		Area* mAreas;
		int mXAreaCount;
		int mYAreaCount;
	};
}


#endif#pragma once
