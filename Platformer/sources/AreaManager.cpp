#include "AreaManager.h"
#include "Area.h"
#include "Item.h"
#include <algorithm>

using namespace Platformer;

void AreaManager::setLevelSize(Size pLevelSize)
{
	mLevelSize = pLevelSize;
}

void AreaManager::init()
{
	mAreaCountX = mLevelSize.width / AREA_SIZE;
	if (mLevelSize.width % AREA_SIZE != 0) mAreaCountX++;
	
	mAreaCountY = mLevelSize.height / AREA_SIZE;
	if (mLevelSize.height % AREA_SIZE != 0) mAreaCountY++;

	mAreas = new Area[mAreaCountY * mAreaCountX];
}

void AreaManager::uninit()
{
	delete[] mAreas;
}

void AreaManager::updateItems(std::list<Item*>* pItems)
{
	for (int i = 0; i < mAreaCountX * mAreaCountY; i++)
	{
		mAreas[i].clearItems();
	}

	for (auto lItem : *pItems)
	{
		addItem(lItem);
	}
}

void AreaManager::addItem(Item* pItem)
{
	if (!pItem->getIsSolid())
	{
		//return;
	}

	pItem->clearArea();
	Rectangle mAbsolutHitBox = pItem->getAbsolutHitBox();
	mAbsolutHitBox.x /= AREA_SIZE;
	mAbsolutHitBox.y /= AREA_SIZE;
	mAbsolutHitBox.width /= AREA_SIZE;
	mAbsolutHitBox.height /= AREA_SIZE;

	mAbsolutHitBox.x = std::min(std::max(mAbsolutHitBox.x,0), mAreaCountX);
	mAbsolutHitBox.y = std::min(std::max(mAbsolutHitBox.y, 0), mAreaCountY);
	mAbsolutHitBox.width = std::min(std::max(mAbsolutHitBox.width, 0), mAreaCountX);
	mAbsolutHitBox.height = std::min(std::max(mAbsolutHitBox.height, 0), mAreaCountY);

	for (int y = mAbsolutHitBox.y; y <= mAbsolutHitBox.y + mAbsolutHitBox.height; y++)
	{
		for (int x = mAbsolutHitBox.x; x <= mAbsolutHitBox.x + mAbsolutHitBox.width; x++)
		{
			Area* lArea = &mAreas[y * mAreaCountX + x];
			pItem->addArea(lArea);
			lArea->addItem(pItem);
		}
	}
}