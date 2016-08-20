#include "Area.h"

using namespace Platformer;

void Area::addItem(Item* pItem)
{
	mItems.push_back(pItem);
}

void Area::clearItems()
{
	mItems.clear();
}

std::vector<Item*>* Area::getItems()
{
	return &mItems;
}
