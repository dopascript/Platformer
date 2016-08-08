#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <SDL_Surface.h>
#include <string>
#include <vector>

class SpriteAnimation
{
public:
	SpriteAnimation();
	SpriteAnimation(const SpriteAnimation& pOther);
	SpriteAnimation(std::string pName);

	void setName(std::string pName);
	std::string getName() const;

	void addFrame(int pFrameId);
	std::vector<int> getFrameIds() const;
	int getFrameId(int pTime) const;
protected:
	std::string mName;
	std::vector<int> mFrameIds;
};

#endif