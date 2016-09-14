#include "Area.h"
#include "Item.h"

using namespace Platformer;

void Area::setSizePosition(Rectangle pRectangle)
{
	mRectangle = pRectangle;
}

void Area::init()
{
	mItemsCount = 0;
}

bool Area::isInside(Item* pItem)
{
	return pItem->getAbsolutHitBox().testHit(mRectangle);
}

void Area::addItem(Item* pItem)
{
	for (int i = 0;i < mItemsCount;i++)
	{
		mItems[i]->addItemProximity(pItem);
		pItem->addItemProximity(mItems[i]);
	}
	if (mItemsCount == ITEMS_MAX - 1)
	{
		return;
	}
	mItems[mItemsCount] = pItem;
	mItemsCount++;
}

void Area::clearItems()
{
	mItemsCount = 0;
}

Item** Area::items()
{
	return mItems;
}

int Area::getItemsCount()
{
	return mItemsCount;
}
