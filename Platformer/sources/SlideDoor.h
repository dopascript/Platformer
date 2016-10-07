#ifndef SLIDEDOOR_H
#define SLIDEDOOR_H

#include <json/json.h>
#include "ImageLibrary.h"
#include "Item.h"

namespace Platformer
{
	class SlideDoor : public Item
	{
	public:
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		Size mSize;
	};
}

#endif