#include "Level.h"

#include <algorithm>
#include <SDL.h>
#include <json/json.h>


#include "ImageLibrary.h"
#include "LoadHelper.h"

using namespace Platformer;

void Level::setScreenSize(Size pSize)
{
	mScreenSize = pSize;
	mScreenHitBox.width = mScreenSize.width;
	mScreenHitBox.height = mScreenSize.height;
}

void Level::init()
{

}

void Level::update(unsigned int pTicks)
{
	updateOnScreenValues();
	activeItems();
	for (auto &lItem : mItems)
	{
		if (lItem->getActive())
		{
			lItem->update(pTicks);
		}
	}

	removeItems();
	addItems();

	updateCameraShift();
	updateScreenHitBox();
}

void Level::activeItems()
{
	for (auto &lItem : mItems)
	{
		if (!lItem->getActive() && lItem->getIsOnScreen())
		{
			lItem->active();
		}
	}
}

void Level::addItems()
{
	for (auto &lItemToAdd : mItemsToAdd)
	{
		mItems.push_back(lItemToAdd);
	}
	mItemsToAdd.clear();

	for (auto& lItemToAdd : mItemsToAddToBack)
	{
		mItems.push_front(lItemToAdd);
	}
	mItemsToAddToBack.clear();
}

void Level::removeItems()
{
	for (auto &lItemToRemove : mItemsToRemove)
	{
		for (auto lItem = mItems.begin(); lItem != mItems.end(); lItem++)
		{
			if ((*lItem) == lItemToRemove)
			{
				mItems.erase(lItem);
				break;
			}
		}

		delete lItemToRemove;
	}
	mItemsToRemove.clear();
}

void Level::updateScreenHitBox()
{
	mScreenHitBox.x = mCameraShift.x * -1;
	mScreenHitBox.y = mCameraShift.y * -1;
}

void Level::updateOnScreenValues()
{
	for (auto &lItem : mItems)
	{
		lItem->updateOnScreenValue(mScreenHitBox);
	}
}

void Level::draw(SDL_Renderer *pSDL_Renderer, unsigned int pTicks)
{
	SDL_SetRenderDrawColor(pSDL_Renderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 255);
	SDL_RenderClear(pSDL_Renderer);

	mBackDecorsMap->draw(pSDL_Renderer, mCameraShift, mScreenHitBox);
	for (auto &lItem : mItems)
	{
		if (!lItem->getIsOnScreen())
		{
			continue;
		}
		lItem->draw(pSDL_Renderer, mCameraShift, pTicks);
	}
	mFrontDecorsMap->draw(pSDL_Renderer, mCameraShift, mScreenHitBox);
}

void Level::updateCameraShift()
{
	int lCameraShiftX = mPlayerAvatar->getPosition().x - (mScreenSize.width / 2);
	int lCameraShiftY = mPlayerAvatar->getPosition().y - (mScreenSize.width / 2);
	
	int lPixelMapWidth = mCollisionMap->getSize().width * mCollisionMap->getTileSize();
	if (lPixelMapWidth < mScreenSize.width)
	{
		lCameraShiftX = ((mScreenSize.width - lPixelMapWidth) / 2);
	}
	else
	{
		lCameraShiftX = std::min(std::max(lCameraShiftX, 0), lPixelMapWidth - mScreenSize.width);
	}
	lCameraShiftX *= -1;

	int lPixelMapHeight = mCollisionMap->getSize().height * mCollisionMap->getTileSize();
	if (lPixelMapHeight < mScreenSize.height)
	{
		lCameraShiftY = (mScreenSize.height - lPixelMapHeight) / 2;
	}
	else
	{
		lCameraShiftY = std::min(std::max(lCameraShiftY, 0), lPixelMapHeight - mScreenSize.height);
	}
	mCameraShift.x = lCameraShiftX;
	mCameraShift.y = lCameraShiftY * -1;
}

Map* Level::getCollisionMap()
{
	return mCollisionMap;
}

void Level::uninit()
{
	for (auto &lItem : mItems)
	{
		delete lItem;
	}
}

std::list<Item*>* Level::getItems()
{
	return &mItems;
}

PlayerAvatar *Level::getPlayerAvatar()
{
	return mPlayerAvatar;
}

void Level::removeItem(Item *pItem)
{
	mItemsToRemove.push_back(pItem);
}

void Level::addItem(Item *pItem)
{
	mItemsToAdd.push_back(pItem);
}

void Level::addItemToBack(Item *pItem)
{
	mItemsToAddToBack.push_back(pItem);
}

Level *Level::LoadFromFile(std::string pFilePath)
{
	Level *lLevelResult = new Level();
	lLevelResult->init();

	std::string lStrJson = loadTextFile(pFilePath);

	Json::Value lRoot;
	Json::Reader lReader;

	bool lParsingSuccessful = lReader.parse(lStrJson, lRoot, false);
	if (!lParsingSuccessful)
	{
		return nullptr;
	}

	lLevelResult->mBackgroundColor.r = lRoot["BackColorR"].asInt();
	lLevelResult->mBackgroundColor.g = lRoot["BackColorG"].asInt();
	lLevelResult->mBackgroundColor.b = lRoot["BackColorB"].asInt();

	Json::Value lJsonMaps = lRoot["Maps"];
	for (Json::ValueIterator lItrJsonMap = lJsonMaps.begin(); lItrJsonMap != lJsonMaps.end(); lItrJsonMap++) 
	{
		Map *lNewMap = Map::LoadFromJson(*lItrJsonMap);
		if (lNewMap->getName() == "CollisionMap")
		{
			lLevelResult->mCollisionMap = lNewMap;
		}
		else if (lNewMap->getName() == "BackDecorsMap")
		{
			lLevelResult->mBackDecorsMap = lNewMap;
		}
		else if (lNewMap->getName() == "FrontDecorsMap")
		{
			lLevelResult->mFrontDecorsMap = lNewMap;
		}
	}

	Json::Value lJsonItems = lRoot["Items"];
	for (Json::ValueIterator lItrJsonItem = lJsonItems.begin(); lItrJsonItem != lJsonItems.end(); lItrJsonItem++)
	{
		Json::Value lJsonItem = (*lItrJsonItem);

		std::string lItemName = lJsonItem["Name"].asString();
		Item* lNewItem = Item::createItem(lItemName);
		if (lNewItem->typeName() == "PlayerAvatar")
		{
			lLevelResult->mPlayerAvatar = (PlayerAvatar*)lNewItem;
		}

		lNewItem->setLevel(lLevelResult);
		lNewItem->init(lJsonItem);
		lLevelResult->addItem(lNewItem);
	}

	return lLevelResult;
}