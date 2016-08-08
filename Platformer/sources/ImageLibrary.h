#ifndef IMAGE_LIBRARY_H
#define IMAGE_LIBRARY_H

#include <map>
#include <SDL.h>

class ImageLibrary
{
	public:
		static ImageLibrary* getInstance();
		void setRender(SDL_Renderer* pSDL_Renderer);
		SDL_Texture* getImage(std::string pPath);
	private:
		ImageLibrary();

		static ImageLibrary* mInstance;
		std::map<std::string, SDL_Texture*> mLibraray;
		SDL_Renderer* mSDL_Renderer;
};

#endif