#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
{

}

SpriteAnimation::SpriteAnimation(const SpriteAnimation& pOther)
{
	mFrameIds = pOther.getFrameIds();
	mName = pOther.getName();
}

SpriteAnimation::SpriteAnimation(std::string pName)
{
	mName = pName;
}

void SpriteAnimation::setName(std::string pName)
{
	mName = pName;
}

std::string SpriteAnimation::getName() const
{
	return mName;
}

void SpriteAnimation::addFrame(int pFrameId)
{
	mFrameIds.push_back(pFrameId);
}

int SpriteAnimation::getFrameId(int pTime) const
{
	return mFrameIds[(pTime / 33) % mFrameIds.size()];
}

std::vector<int> SpriteAnimation::getFrameIds() const
{
	return mFrameIds;
}