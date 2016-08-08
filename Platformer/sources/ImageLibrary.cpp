#include "ImageLibrary.h"

#include <SDL_image.h>


ImageLibrary* ImageLibrary::mInstance = nullptr;

ImageLibrary::ImageLibrary()
{
	mInstance = nullptr;
}

ImageLibrary* ImageLibrary::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ImageLibrary();
	}
	return mInstance;
}

void ImageLibrary::setRender(SDL_Renderer* pSDL_Renderer)
{
	mSDL_Renderer = pSDL_Renderer;
}

SDL_Texture* ImageLibrary::getImage(std::string pPath)
{
	if (mLibraray.find(pPath) == mLibraray.end())
	{
		SDL_Surface* surface = IMG_Load(pPath.c_str());
		mLibraray[pPath] = SDL_CreateTextureFromSurface(mSDL_Renderer, surface);
	}

	return mLibraray[pPath];
}