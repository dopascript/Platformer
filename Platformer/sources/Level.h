#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <vector>
#include <SDL.h>
#include "PlayerAvatar.h"
#include "Item.h"
#include "Map.h"
#include "Color.h"
#include "Camera.h"
#include "AreaManager.h"

class Game;

namespace Platformer
{
	class Level
	{
	public:
		void setGame(Game* pGame);
		void setScreenSize(Size pSize);
		void setRenderer(SDL_Renderer* pRenderer);

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

		void setLevelToLoad(std::string pLevelPath, Point pAvatarPosition);

		static Level *LoadFromFile(std::string pFilePath, Point pAvatarPosition);

		SDL_Texture* getRenderTexture();
	protected:
		void activeItems();
		void removeItems();
		void addItems();
		void updateScreenHitBox();
		void updateOnScreenValues();

		SDL_Renderer* mRenderer;
		SDL_Texture* mRenderTexture;

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
		Camera mCamera;
		AreaManager mAreaManager;

		std::string mLevelToLoad;
		Point mReloadAvatarPosition;

		Game* mGame;
	};
}
#endif