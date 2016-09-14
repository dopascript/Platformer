#ifndef DUPLICATOR_H
#define DUPLICATOR_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class Duplicator : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
		virtual void onAvatarProximity(unsigned int pTime, Item* pAvatar);
	protected:
		bool mUsed;
		Point mNewAvatarPosition;
	};
}

#endif