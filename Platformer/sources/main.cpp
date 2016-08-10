#include <stdio.h>
#include <SDL.h>
#include "Level.h"
#include "ImageLibrary.h"
#include "Input.h"
#include "Item.h"

int main(int argc, char** argv)
{
	

	bool quit = false;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Mario Bros", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1024,
		768,
		SDL_WINDOW_SHOWN);

	SDL_Renderer *Main_Renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	ImageLibrary::getInstance()->setRender(Main_Renderer);



	Platformer::Item::itemsTypesListing();

	Platformer::Level *level = Platformer::Level::LoadFromFile("levels/level1.json");
	level->setScreenSize(Size(1024, 768));

	SDL_Event event;
	while (true)
	{
		Input::getInstance()->update();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
		}
		if (quit)
		{
			break;
		}

		unsigned int lTicks = SDL_GetTicks();
		
		level->update(lTicks);
		level->draw(Main_Renderer, lTicks);

		SDL_RenderPresent(Main_Renderer);

		int lMillisecondsToWait = 33 - (SDL_GetTicks() - lTicks);
		if (lMillisecondsToWait > 0)
		{
			SDL_Delay(lMillisecondsToWait);
		}
	}


	SDL_DestroyWindow(pWindow);

	SDL_Quit();

	return 0;
}