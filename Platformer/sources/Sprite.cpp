#include "Sprite.h"

#include <algorithm>
#include <vector>

Sprite::Sprite()
{
	mCurrentFrame = 0;
	mSpriteCountY = 100;
}

Sprite::Sprite(const Sprite& pOther)
{
	mTexture = pOther.getTexture();
	mSize    = pOther.getSize();
	mOrigin  = pOther.getOrigin();
	mSpriteCountY = pOther.getSpriteCountY();
	for (auto& lAnimation : pOther.getAnimations())
	{
		addAnimation(lAnimation);
	}
}

Sprite& Sprite::operator= (const Sprite& pOther)
{
	if (this != &pOther)
	{
		this->mTexture = pOther.getTexture();
		this->mSize = pOther.getSize();
		this->mOrigin = pOther.getOrigin();
		this->mSpriteCountY = pOther.getSpriteCountY();
		for (auto& lAnimation : pOther.getAnimations())
		{
			this->addAnimation(lAnimation);
		}
	}
	return *this;
}

void Sprite::setTexture(SDL_Texture* pTexture)
{
	mTexture = pTexture;
}

SDL_Texture* Sprite::getTexture() const
{
	return mTexture;
}

void Sprite::setSize(int pWidth, int pHeight)
{
	mSize = Size(pWidth, pHeight);
}

void Sprite::setSize(Size pSize)
{
	mSize = pSize;
}

Size Sprite::getSize() const
{
	return mSize;
}

void Sprite::setOrigin(Point pOrigin)
{
	mOrigin = pOrigin;
}

void Sprite::setSpriteCountY(int pSpriteCountY)
{
	mSpriteCountY = pSpriteCountY;
}

int Sprite::getSpriteCountY() const
{
	return mSpriteCountY;
}

Point Sprite::getOrigin() const
{
	return mOrigin;
}

void Sprite::setHorizontalFlip(bool pFlip)
{
	mFlip = pFlip;
}

bool Sprite::getHorizontalFlip()
{
	return mFlip;
}

void Sprite::addAnimation(SpriteAnimation& pSpriteAnimation)
{
	mSpriteAnimations[pSpriteAnimation.getName()] = pSpriteAnimation;
}

std::vector<SpriteAnimation> Sprite::getAnimations() const
{
	std::vector<SpriteAnimation> result;
	for (auto& lAnimation : mSpriteAnimations)
	{
		result.push_back(lAnimation.second);
	}
	return result;
}


void Sprite::playAnimation(std::string pName)
{
	mCurrentAnimation = &mSpriteAnimations[pName];
}

void Sprite::setFrame(int pFrameId)
{
	mCurrentAnimation = nullptr;
	mCurrentFrame     = pFrameId;
}

Rectangle Sprite::getTextureRectangle(int pTime)
{
	int lFrame = mCurrentFrame;
	if (mCurrentAnimation != nullptr)
	{
		lFrame = mCurrentAnimation->getFrameId(pTime);
	}
	int lX = lFrame % mSpriteCountY;
	int lY = lFrame / mSpriteCountY;
	return Rectangle(mSize.width, mSize.height, lX * mSize.width, lY * mSize.height);
}

Rectangle Sprite::getOnScreenRectangle(Point pPosition)
{
	return Rectangle(mSize.width, mSize.height, pPosition.x - mOrigin.x, pPosition.y - mOrigin.y);
}