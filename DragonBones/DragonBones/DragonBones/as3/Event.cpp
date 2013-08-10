#include "Event.h"

namespace DragonBones {
	Event::Event(std::string type) : Object("Event") {
		this->type = type;
	}

	Event::~Event() {
		type = "";
	}
}