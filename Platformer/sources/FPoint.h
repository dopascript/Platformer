#ifndef FPOINT_H
#define FPOINT_H

#include <SDL.h>

class FPoint
{
public:
	float x;
	float y;

	FPoint()
	{
		x = 0;
		y = 0;
	}

	FPoint(float pX, float pY)
	{
		x = pX;
		y = pY;
	}

	FPoint(const FPoint& pOther)
	{
		x = pOther.x;
		y = pOther.y;
	}

	SDL_Point toSDL_Point()
	{
		SDL_Point lResult;
		lResult.x = (int)x;
		lResult.y = (int)y;
		return lResult;
	}
};

#endif