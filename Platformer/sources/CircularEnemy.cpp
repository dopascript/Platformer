#include "CircularEnemy.h"
#include "ImageLibrary.h"
#include "Level.h"
#include "PlayerAvatar.h"
#include "Item.h"
#include <math.h>

using namespace Platformer;

CircularEnemy::CircularEnemy()
{

}

void CircularEnemy::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/items.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(24, 24));
	lPlayerSprite.setSpriteCountY(10);
	lPlayerSprite.setOrigin(Point(12, 12));
	 
	setSprite(lPlayerSprite);

	mSprite.setFrame(62);

	setHitBox(Rectangle(16, 16, 0, -1));

	mAngle = 0;
}

void CircularEnemy::init(Json::Value pJson)
{
	init();

	mRadius = pJson["Radius"].asInt();
	mRotationRadius = pJson["RotationRadius"].asInt();
	int lRotationX = pJson["X"].asInt();
	int lRotationY = pJson["Y"].asInt();
	mRotationCenter = Point(lRotationX, lRotationY);
	mSpeed = pJson["Speed"].asFloat();
	setPosition(Point(lRotationX - mRadius, lRotationY));
}

void CircularEnemy::update(unsigned int pTicks)
{
	mAngle += 0.05f;
	int lX = mRotationCenter.x + (int)(std::cos(mAngle) * mRotationRadius);
	int lY = mRotationCenter.y + (int)(std::sin(mAngle) * mRotationRadius);
	setPosition(Point(lX, lY));

	for (auto &lPlayerAvatar : *mLevel->getPlayerAvatars())
	{
		if (lPlayerAvatar->getAbsolutHitBox().testHit(Circle(lX, lY, mRadius)))
		{
			lPlayerAvatar->hit(pTicks);
		}
	}

	Item::update(pTicks);
}

std::string CircularEnemy::typeName()
{
	return "CircularEnemy";
}

Item* CircularEnemy::createItem()
{
	return new CircularEnemy();
}
