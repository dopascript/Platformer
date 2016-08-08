#ifndef MOBILEPLATFORM_H
#define MOBILEPLATFORM_H

#include <vector>
#include <json/json.h>

#include "Item.h"

namespace Platformer
{
	struct MobilePlatformTrigger
	{
		Point mTriggerPosition;
		FPoint mDirection;
	};

	class MobilePlatform : public Item
	{
	public:
		void setStartPosition(Point pStartPosition);
		void init();
		virtual void init(Json::Value pJson);
		virtual void update(unsigned int pTicks);
		void addMobilePlatformTrigger(MobilePlatformTrigger pTrigger);
		virtual std::string typeName();
		virtual Item* createItem();
	protected:
		Point mStartPosition;
		std::vector<MobilePlatformTrigger> mTriggers;

	};
}

#endif