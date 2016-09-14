#include "AreaManager.h"
#include "Area.h"
#include <algorithm>

using namespace Platformer;

void AreaManager::setLevelSize(Size pLevelSize)
{
	mLevelSize = pLevelSize;
}

void AreaManager::init()
{
	mXAreaCount = mLevelSize.width / AREA_SIZE;
	if (mLevelSize.width % AREA_SIZE > 0) mXAreaCount++;

	mYAreaCount = mLevelSize.height / AREA_SIZE;
	if (mLevelSize.height % AREA_SIZE > 0) mYAreaCount++;

	int lAreasCount = mXAreaCount * mYAreaCount;
	mAreas = new Area[lAreasCount];
	for (int i = 0; i < lAreasCount; i++)
	{
		mAreas[i].init();
	}
}

void AreaManager::updateItems(std::list<Item*>* pItems)
{
	int mAreasCount = mXAreaCount * mYAreaCount;
	for (int i = 0; i < mAreasCount; i++)
	{
		mAreas[i].clearItems();
	}

	for(auto item : *pItems)
	{
		updateItem(item);
	}
}

void AreaManager::updateItem(Item* pItem)
{
	pItem->clearItemsProximity();

	Rectangle lAbsolutHitBox = pItem->getAbsolutHitBox();

	Area* area[4];

	int lX1 = std::min(std::max(lAbsolutHitBox.x - 10, 0), mLevelSize.width - 1);
	int lX2 = std::min(std::max(lAbsolutHitBox.x + lAbsolutHitBox.width + 10, 0), mLevelSize.width - 1);
	int lY1 = std::min(std::max(lAbsolutHitBox.y - 10, 0), mLevelSize.height - 1);
	int lY2 = std::min(std::max(lAbsolutHitBox.y + lAbsolutHitBox.height + 10, 0), mLevelSize.height - 1);

	area[0] = getAreaAtPosition(Point(lX1, lY1));
	area[1] = getAreaAtPosition(Point(lX2, lY1));
	area[2] = getAreaAtPosition(Point(lX1, lY2));
	area[3] = getAreaAtPosition(Point(lX2, lY2));

	if (area[0] == area[1] && area[1] == area[2] && area[2] == area[3])
	{
		area[0]->addItem(pItem);
	}
	else if (area[0] == area[1] && area[2] == area[3])
	{
		area[0]->addItem(pItem);
		area[2]->addItem(pItem);
	}
	else if (area[0] == area[2] && area[1] == area[3])
	{
		area[0]->addItem(pItem);
		area[1]->addItem(pItem);
	}
	else
	{
		area[0]->addItem(pItem);
		area[1]->addItem(pItem);
		area[2]->addItem(pItem);
		area[3]->addItem(pItem);
	}
}

Area* AreaManager::getAreaAtPosition(Point pPosition)
{
	int x = pPosition.x / AREA_SIZE;
	int y = pPosition.y / AREA_SIZE;
	return &mAreas[y * mXAreaCount + x];
}

void AreaManager::uninit()
{

}
