#ifndef AREA_H
#define AREA_H

#include <list>
#include "MainMenu.h"
#include "Rectangle.h"
#include "Size.h"

#define ITEMS_MAX 100

namespace Platformer
{
	class Item;

	class Area
	{
	public:
		void setSizePosition(Rectangle pRectangle);
		void init();
		bool isInside(Item* pItem);

		void addItem(Item* pItem);
		void clearItems();

		Item** items();
		int getItemsCount();
	protected:
		Rectangle mRectangle;
		Item* mItems[ITEMS_MAX];
		int mItemsCount;
	};
}


#endif
