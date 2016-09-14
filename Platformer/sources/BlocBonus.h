#ifndef BLOCBONUS_H
#define BLOCBONUS_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class BlocBonus : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
		virtual void onAvatarProximity(unsigned int pTime, Item* pAvatar);
	protected:

		void createBonus(unsigned int pTicks);

		FPoint mAcceleration;
		Point mInitialPosition;
	};
}

#endif