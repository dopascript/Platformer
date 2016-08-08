#ifndef ENEMYMUSHROOM_H
#define ENEMYMUSHROOM_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class EnemyMushroom : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		void hit(unsigned int pTime);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		void updatePlayerHit(unsigned int pTime);

		bool mIsDead;
		int mKillTime;
	};
}

#endif