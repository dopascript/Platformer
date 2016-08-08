#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL.h>

#include "Size.h"
#include "Point.h"


class Rectangle
{
public:
	int x;
	int y;
	int width;
	int height;

	Rectangle()
	{
		x = 0;
		y = 0;;
		width = 1;
		height = 1;
	}

	Rectangle(int pWidth, int pHeight, int pX, int pY)
	{
		x = pX;
		y = pY;
		width = pWidth;
		height = pHeight;
	}

	Rectangle(const Rectangle& pOther)
	{
		x = pOther.x;
		y = pOther.y;
		width = pOther.width;
		height = pOther.height;
	}

	bool testHit(const Rectangle& pOther)
	{
		return x < pOther.x + pOther.width &&
			x + width > pOther.x &&
			y < pOther.y + pOther.height &&
			y + height > pOther.y;
	}

	SDL_Rect toSDL_Rect()
	{
		SDL_Rect lResult;
		lResult.x = x;
		lResult.y = y;
		lResult.w = width;
		lResult.h = height;
		return lResult;
	}
};

#endif