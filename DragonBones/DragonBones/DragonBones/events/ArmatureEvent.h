#ifndef DRAGONBONE_ARMATUREEVENT_H
#define DRAGONBONE_ARMATUREEVENT_H

#include <string>
#include "../as3/Event.h"

namespace DragonBones {
	class ArmatureEvent : public Event{
	public:
		static const std::string Z_ORDER_UPDATED;
		ArmatureEvent(std::string type);
		Event* clone();
	};
}

#endif