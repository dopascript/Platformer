#ifndef AREA_H
#define AREA_H

#include <vector>

namespace Platformer
{
	class Item;

	class Area
	{
	public:
		void addItem(Item* pItem);
		void clearItems();
		std::vector<Item*>* getItems();
	private:
		std::vector<Item*> mItems;
	};
}

#endif