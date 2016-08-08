#ifndef BONUSMUSHROOM_H
#define BONUSMUSHROOM_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class BonusMushroom : public Item
	{
	public:
		BonusMushroom();
		void init();
		virtual void init(Json::Value pJson);
		void setBlocOrigine(Item *pBlocOrigine);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		Item* mBlocOrigine;
	};
}

#endif