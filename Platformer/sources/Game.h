#ifndef GAME_H
#define GAME_H

#include <json/json.h>
#include <string>
#include <SDL.h>
#include "Level.h"
#include "MainMenu.h"

class Game
{
public:
	void setWindowSize(Size pSize);
	void setRenderer(SDL_Renderer* pRenderer);

	void init();
	void update(unsigned int pTicks);
	void draw(SDL_Renderer* pSDL_Renderer, unsigned int pTicks);
	void uninit();

	void startPlatformLevel(std::string mPath);
protected:
	SDL_Renderer* mRenderer;
	Platformer::Level* mPlatformLevel;
	Menu::MainMenu* mMainMenu;
	Size mScreenSize;
};

#endif
