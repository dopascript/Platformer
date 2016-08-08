#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <map>
#include <SDL.h>

namespace Menu
{
	class MainMenu
	{
	public:
		MainMenu();

		void init();
		void update(unsigned int pTicks);
		void draw(SDL_Renderer* pSDL_Renderer, unsigned int pTicks);
		void uninit();
	private:

	};
}

#endif