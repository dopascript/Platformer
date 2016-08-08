#include "ItemHelper.h"
#include "Item.h"

namespace Platformer
{
	void ItemClassicMove(Level *pLevel, Item *pItem)
	{
		float lDirection = pItem->getSpeed().x;
		Point lPosition = pItem->getPosition();

		Point lNextPosition(lPosition.x + (int)lDirection, lPosition.y);
		Point lNextUpPosition(lPosition.x + (int)lDirection, lPosition.y - (int)std::abs(lDirection));
		if (pItem->testHit(lNextPosition, FPoint(lDirection, 5), CollisionTest_MapAndItems) &&
			pItem->testHit(lNextUpPosition, FPoint(lDirection, 5), CollisionTest_MapAndItems))
		{
			lDirection *= -1.0f;
		}
		FPoint lNewSpeed(lDirection, 5);

		pItem->setSpeed(lNewSpeed);

		pItem->move(pItem->getSpeed(), CollisionTest_MapAndItems);
		if (std::abs(pItem->getSpeed().x) < 1.0f)
		{
			pItem->setSpeed(FPoint(lDirection, pItem->getSpeed().y));
		}
	}
}