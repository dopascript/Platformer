#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	}

	Color(unsigned char pRed, unsigned char pGreen, unsigned char pBlue, unsigned char pAlpha)
	{
		r = pRed;
		g = pGreen;
		b = pBlue;
		a = pAlpha;
	}

	Color(const Color& pOther)
	{
		r = pOther.r;
		g = pOther.g;
		b = pOther.b;
		a = pOther.a;
	}

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

#endif