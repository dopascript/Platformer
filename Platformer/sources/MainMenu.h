#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <map>
#include <SDL.h>
//#include "Game.h"

class Game;

namespace Menu
{
	class MainMenu
	{
	public:
		MainMenu();
		
		void setGame(Game* pGame);

		void init();
		void update(unsigned int pTicks);
		void draw(SDL_Renderer* pSDL_Renderer, unsigned int pTicks);
		void uninit();
	protected:
		Game* mGame;
	};
}

#endif