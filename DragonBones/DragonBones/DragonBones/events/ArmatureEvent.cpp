#include "ArmatureEvent.h"

namespace DragonBones {
	const std::string ArmatureEvent::Z_ORDER_UPDATED = "zOrderUpdated";

	ArmatureEvent::ArmatureEvent(std::string type) : Event(type)
	{
	}

	Event* ArmatureEvent::clone()
	{
		return new ArmatureEvent(type);
	}
}