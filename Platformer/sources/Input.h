#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL.h>

class Input
{
public:
	static Input* getInstance();
	void update();
	bool isKeyDown(SDL_Keycode pKeyCode);
	bool isPress(SDL_Keycode pKeyCode);
private:
	Input();

	static Input* mInstance;
	const Uint8* mKeyboard;
	Uint8* mLastStateKeyboard;
};

#endif