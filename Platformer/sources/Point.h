#ifndef POINT_H
#define POINT_H

#include <SDL.h>

class Point
{
public:
	int x;
	int y;

	Point()	
	{
		x = 0;
		y = 0; 
	}

	Point(int pX, int pY)
	{ 
		x = pX;
		y = pY; 
	}

	Point(const Point &pOther)
	{
		x = pOther.x;
		y = pOther.y;
	}

	SDL_Point toSDL_Point()
	{
		SDL_Point lResult;
		lResult.x = x;
		lResult.y = y;
		return lResult;
	}

	float distance(const Point &pOther)
	{
		return (float)std::sqrt((x - pOther.x)*(x - pOther.x) + (y - pOther.y)*(y - pOther.y));
	}
};

#endif