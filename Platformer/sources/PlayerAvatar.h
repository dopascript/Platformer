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
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		void kill(unsigned int pTicks);
		void updateSpecialAppearance(unsigned int pTicks);

		PlayerAvatarForm mPlayerAvatarForm;

		int mJumpFrame;
		int mWaitFrame;
		std::string mRunAnimation;

		bool mIsDead;

		bool mIsUntouchable;
		int mUntouchableStartTime;
		int mUntouchableRemainingTime;

		int mLastTailUseTime;
	};
}

#endif