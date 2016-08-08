#include "Input.h"
#include <SDL.h>

Input* Input::mInstance = nullptr;

Input::Input()
{
	mLastStateKeyboard = nullptr;
	mKeyboard = nullptr;
}

Input* Input::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Input();
	}
	return mInstance;
}

void Input::update()
{
	if (mKeyboard == nullptr)
	{
		mKeyboard = SDL_GetKeyboardState(NULL);
	}
	else
	{
		mLastStateKeyboard = new Uint8[200];
		memcpy(mLastStateKeyboard, mKeyboard, 200);
	}
	SDL_PumpEvents();
}

bool Input::isKeyDown(SDL_Keycode pKeyCode)
{
	return mKeyboard[pKeyCode] == 1;
}

bool Input::isPress(SDL_Keycode pKeyCode)
{
	if (mLastStateKeyboard == nullptr)
	{
		return false;
	}
	return mKeyboard[pKeyCode] == 1 && mKeyboard[pKeyCode] != mLastStateKeyboard[pKeyCode];
}