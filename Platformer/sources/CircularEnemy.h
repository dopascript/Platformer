#ifndef CIRCULARENEMY_H
#define CIRCULARENEMY_H

#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	class CircularEnemy : public Item
	{
	public:
		CircularEnemy();
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		Point mRotationCenter;
		int mRotationRadius;
		int mRadius;
		float mSpeed;
		float mAngle;
	};
}

#endif