#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <vector>
#include <SDL.h>
#include "PlayerAvatar.h"
#include "Item.h"
#include "Map.h"
#include "Color.h"

namespace Platformer
{
	class Level
	{
	public:

		void setScreenSize(Size pSize);

		void init();
		void update(unsigned int pTicks);
		void draw(SDL_Renderer *pSDL_Renderer, unsigned int pTicks);
		void uninit();

		std::list<PlayerAvatar*>* getPlayerAvatars();

		std::list<Item*>* getItems();

		Map* getCollisionMap();

		void removeItem(Item *pItem);
		void addItem(Item *pItem);
		void addItemToBack(Item *pItem);

		static Level *LoadFromFile(std::string pFilePath);
	protected:
		void activeItems();
		void removeItems();
		void addItems();
		void updateCameraShift();
		void updateScreenHitBox();
		void updateOnScreenValues();

		std::list<PlayerAvatar*> mPlayerAvatars;
		std::list<Item*> mItems;

		std::vector<Item*> mItemsToRemove;
		std::vector<Item*> mItemsToAdd;
		std::vector<Item*> mItemsToAddToBack;

		Map* mCollisionMap;
		Map* mBackDecorsMap;
		Map* mFrontDecorsMap;

		Color mBackgroundColor;

		Size mScreenSize;
		Rectangle mScreenHitBox;
		Point mCameraShift;

		
	};
}
#endif