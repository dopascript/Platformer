#ifndef CIRCLE_H
#define CIRCLE_H

class Circle
{
public:
	int x;
	int y;
	int radius;

	Circle()
	{
		x = 0;
		y = 0;
		radius = 1;
	}

	Circle(int pX, int pY, int pRadius)
	{
		x = pX;
		y = pY;
		radius = pRadius;
	}

	Circle(const Circle &pOther)
	{
		x = pOther.x;
		y = pOther.y;
		radius = pOther.radius;
	}
};

#endif