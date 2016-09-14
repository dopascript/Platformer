#include "CoinBonus.h"
#include "ImageLibrary.h"
#include "Level.h"
#include "SoundPlayer.h"

using namespace Platformer;

void CoinBonus::init()
{
	SDL_Texture* lPlayerTexture = ImageLibrary::getInstance()->getImage("images/map_decor.png");
	Sprite lPlayerSprite;
	lPlayerSprite.setTexture(lPlayerTexture);
	lPlayerSprite.setSize(Size(16, 16));
	lPlayerSprite.setOrigin(Point(0, 0));
	lPlayerSprite.setSpriteCountY(29);

	setSprite(lPlayerSprite);


	SpriteAnimation lAnimation("coin");
	lAnimation.addFrame(609);
	lAnimation.addFrame(609);
	lAnimation.addFrame(609);
	lAnimation.addFrame(610);
	lAnimation.addFrame(610);
	lAnimation.addFrame(610);
	lAnimation.addFrame(611);
	lAnimation.addFrame(611);
	lAnimation.addFrame(611);
	lAnimation.addFrame(612);
	lAnimation.addFrame(612);
	lAnimation.addFrame(612);
	lPlayerSprite.addAnimation(lAnimation);
	setSprite(lPlayerSprite);

	mSprite.playAnimation("coin");

	setHitBox(Rectangle(16, 16, 0, -1));

	mSpeed = FPoint(0.0f, 0.0f);
	mIsSolid = false;

	SoundPlayer::getInstance()->addSound("coin","sounds/coin.wav");
}

void CoinBonus::init(Json::Value pJson)
{
	init();

	int lX = pJson["X"].asInt();
	int lY = pJson["Y"].asInt();
	setPosition(Point(lX, lY));
}

void CoinBonus::update(unsigned int pTicks)
{

}

void CoinBonus::onAvatarProximity(unsigned int pTime, Item* pAvatar)
{
	PlayerAvatar* lPlayerAvatar = (PlayerAvatar*)pAvatar;
	Rectangle lHitbox = lPlayerAvatar->getAbsolutHitBox();
	if (getAbsolutHitBox().testHit(lHitbox))
	{
		mLevel->removeItem(this);
		SoundPlayer::getInstance()->playSound("coin");
		return;
	}
}

std::string CoinBonus::typeName()
{
	return "CoinBonus";
}

Item* CoinBonus::createItem()
{
	return new CoinBonus();
}