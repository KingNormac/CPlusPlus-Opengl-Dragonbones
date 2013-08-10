#ifndef DRAGONBONES_EVENT_H
#define DRAGONBONES_EVENT_H

#include <string>
#include "Object.h"

namespace DragonBones {
	class Event : public Object {
	public:
		std::string type;
		Event(std::string type);
		~Event();
		Object* target;
	};

}

#endif