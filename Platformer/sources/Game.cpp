#include "Game.h"

void Game::setRenderer(SDL_Renderer* pRenderer)
{
	mRenderer = pRenderer;
}

void Game::setWindowSize(Size pSize)
{
	pScreenSize = pSize;
}

void Game::init()
{
	mPlatformLevel = nullptr;
	mMainMenu = new Menu::MainMenu();
	mMainMenu->init();
}

void Game::update(unsigned int pTicks)
{
	if (mPlatformLevel != nullptr)
	{
		mPlatformLevel->update(pTicks);
	}
	else
	{
		mMainMenu->update(pTicks);
	}
}

void Game::draw(SDL_Renderer *pSDL_Renderer, unsigned int pTicks)
{
	if (mPlatformLevel != nullptr)
	{
		mPlatformLevel->draw(pSDL_Renderer, pTicks);
		SDL_SetRenderTarget(mRenderer, NULL);

		SDL_Rect destination;
		destination.x = 0;
		destination.y = 0;
		destination.w = pScreenSize.width;
		destination.h = pScreenSize.height;
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = pScreenSize.width / 2;
		rect.h = pScreenSize.height / 2;
		SDL_RenderCopy(pSDL_Renderer, mPlatformLevel->getRenderTexture(), &rect, &destination);
	}
	else
	{
		mMainMenu->draw(pSDL_Renderer, pTicks);
	}
}

void Game::uninit()
{
	mMainMenu->uninit();
}

void Game::startPlatformLevel(std::string mPath)
{
	mPlatformLevel = Platformer::Level::LoadFromFile(mPath);
	mPlatformLevel->setRenderer(mRenderer);
	mPlatformLevel->setScreenSize(Size(pScreenSize.width / 2, pScreenSize.height / 2));
	mPlatformLevel->setGame(this);
	mPlatformLevel->init();
}