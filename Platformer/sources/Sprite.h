#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <map>
#include <vector>

#include <SDL_Surface.h>

#include "SpriteAnimation.h"
#include "Point.h"
#include "Rectangle.h"

class Sprite
{
public:
	Sprite();
	Sprite(const Sprite& pOther);
	Sprite& operator= (const Sprite& pOther);

	void setTexture(SDL_Texture* pTexture);
	SDL_Texture* getTexture() const;

	void setSize(int pWidth, int pHeight);
	void setSize(Size pSize);
	Size getSize() const;

	void setOrigin(Point pOrigin);
	Point getOrigin() const;

	void setHorizontalFlip(bool pFlip);
	bool getHorizontalFlip();

	void setSpriteCountY(int pSpriteCountY);
	int getSpriteCountY() const;

	void addAnimation(SpriteAnimation& pSpriteAnimation);
	std::vector<SpriteAnimation> getAnimations() const;

	void playAnimation(std::string pName);
	void setFrame(int pFrameId);

	Rectangle getTextureRectangle(int pTime);
	Rectangle getOnScreenRectangle(Point pPosition);
protected:
	SDL_Texture* mTexture;
	Size mSize;
	int mSpriteCountY;
	Point mOrigin;
	bool mFlip;
	std::map<std::string, SpriteAnimation> mSpriteAnimations;

	SpriteAnimation* mCurrentAnimation;
	int mCurrentFrame;
};

#endif