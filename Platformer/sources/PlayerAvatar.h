#ifndef PLAYERAVATAR_H
#define PLAYERAVATAR_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	enum PlayerAvatarForm { PlayerAvatarState_Tiny, PlayerAvatarState_Big, PlayerAvatarState_Raccoon, CollisionTest_Superball };

	class PlayerAvatar : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		void updateMoves(unsigned int pTicks);
		void setForm(PlayerAvatarForm pPlayerAvatarForm);
		bool isDead();
		void hit(unsigned int pTicks);
		void setKilling(unsigned int pTime);
		bool isKilling(unsigned int pTime);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		void kill(unsigned int pTicks);
		void updateSpecialAppearance(unsigned int pTicks);

		PlayerAvatarForm mPlayerAvatarForm;

		int mJumpFrame;
		int mWaitFrame;
		int mCrouchedFrame;
		int mSliddingFrame;
		std::string mRunAnimation;

		bool mIsDead;
		int mDeathTime;

		bool mUntouchable;
		int mUntouchableStartTime;
		int mUntouchableRemainingTime;

		bool mUsingParachute;
		bool mCrouched;
		bool mSlidding;
		unsigned int mKillingTime;
	};
}

#endif