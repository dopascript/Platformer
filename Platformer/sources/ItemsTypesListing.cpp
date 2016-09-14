#include "Item.h"
#include "MobilePlatform.h"
#include "EnemyMushroom.h"
#include "BonusMushroom.h"
#include "BlocBonus.h"
#include "PlayerAvatar.h"
#include "CoinBonus.h"
#include "CircularEnemy.h"
#include "Door.h"
#include "Duplicator.h"

using namespace Platformer;

void Item::itemsTypesListing()
{
	addItemsType(new MobilePlatform());
	addItemsType(new EnemyMushroom());
	addItemsType(new BonusMushroom());
	addItemsType(new BlocBonus());
	addItemsType(new PlayerAvatar());
	addItemsType(new CoinBonus());
	addItemsType(new CircularEnemy());
	addItemsType(new Door());
	addItemsType(new Duplicator());
}

void Item::addItemsType(Item *pItem)
{
	if (mItemsTypes.find(pItem->typeName()) == mItemsTypes.end())
	{
		mItemsTypes[pItem->typeName()] = pItem;
	}
}

Item* Item::createItem(std::string pName)
{
	if (mItemsTypes.find(pName) != mItemsTypes.end())
	{
		return mItemsTypes[pName]->createItem();
	}
	return nullptr;
}

bool Item::itemTypeExist(std::string pName)
{
	return mItemsTypes.find(pName) != mItemsTypes.end();
}