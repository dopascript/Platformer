#ifndef SIZE_H
#define SIZE_H

class Size
{
public:
	int width;
	int height;

	Size()
	{
		width = 0;
		height = 0;
	}

	Size(int pWidth, int pHeight)
	{
		width = pWidth;
		height = pHeight;
	}

	Size(const Size& pOther)
	{
		width = pOther.width;
		height = pOther.height;
	}	
};

#endif