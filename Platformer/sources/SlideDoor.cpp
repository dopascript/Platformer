#include "SlideDoor.h"
#include "SoundPlayer.h"
#include "Level.h"

using namespace Platformer;

void SlideDoor::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/map_decor.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(16, 16));
	lPlayerSprite.setOrigin(Point(0, 0));
	lPlayerSprite.setSpriteCountY(29);

	setSprite(lPlayerSprite);


	SpriteAnimation lAnimation("bloc");
	lAnimation.addFrame(580);
	lAnimation.addFrame(580);
	lAnimation.addFrame(580);
	lAnimation.addFrame(581);
	lAnimation.addFrame(581);
	lAnimation.addFrame(581);
	lAnimation.addFrame(582);
	lAnimation.addFrame(582);
	lAnimation.addFrame(582);
	lAnimation.addFrame(583);
	lAnimation.addFrame(583);
	lAnimation.addFrame(583);
	lPlayerSprite.addAnimation(lAnimation);
	setSprite(lPlayerSprite);
	
	setHitBox(Rectangle(16, 16, 0, -1));

	mSpeed = FPoint(0.0f, 0.0f);

	mIsSolid = true;
}

void SlideDoor::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));

	mSize.width = pJson["Width"].asInt();
	mSize.height = pJson["Height"].asInt();

	setHitBox(Rectangle(mSize.width, mSize.height, 0, 0));

	mSprite.setSize(mSize);
}

void SlideDoor::update(unsigned int pTicks)
{
	Item::update(pTicks);
}

std::string SlideDoor::typeName()
{
	return "SlideDoor";
}

Item* SlideDoor::createItem()
{
	return new SlideDoor();
}
