#ifndef DOOR_H
#define DOOR_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class Door : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
		virtual void onAvatarProximity(unsigned int pTime, Item* pAvatar);
	protected:
		std::string mLevelPath;
		Point mAvatarPosition;
	};
}

#endif