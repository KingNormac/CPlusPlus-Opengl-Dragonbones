#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

/**
My custom version of a event dispatcher for c++ for dragonbones
I had to inherit IAnimatable so that Armature could contain both IAnimatable and EventDispatcher
**/

#include <string>
#include <map>
#include <vector>
#include "Object.h"

namespace DragonBones {
	class Event;

	typedef void (*function)(Event*);

	class EventDispatcher : public Object {
	public:
		std::map<std::string, function> eventListeners;
		static std::vector<EventDispatcher*> listOfDispatchers;

		EventDispatcher();

		~EventDispatcher();

		void addEventListener(std::string type, function func);
		bool dispatchEvent(Event* value);
		bool hasEventListener(std::string type);
		void removeEventListener(std::string type);

	};
}


#endif