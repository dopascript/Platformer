#ifndef COINBONUS_H
#define COINBONUS_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class CoinBonus : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:

	};
}

#endif