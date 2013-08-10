#include "EventDispatcher.h"
#include "Event.h"

namespace DragonBones {

	std::vector<EventDispatcher*> EventDispatcher::listOfDispatchers =  std::vector<EventDispatcher*>(0);

	EventDispatcher::EventDispatcher() : Object("EventDispatcher")
	{
		listOfDispatchers.push_back(this);
	}

	EventDispatcher::~EventDispatcher(){}

	void EventDispatcher::addEventListener(std::string type, function func)
	{
		eventListeners.insert(std::make_pair(type, func));
	}

	bool EventDispatcher::dispatchEvent(Event* value) {
		value->target = this;
		//for(EventDispatcher* dispatcher : listOfDispatchers) {
		//	if(dispatcher) {
		if(hasEventListener(value->type)) {
				eventListeners.find(value->type)->second(value);
		}
		return true;
	}

	bool EventDispatcher::hasEventListener(std::string type) {
		if(eventListeners.find(type.c_str()) == eventListeners.end()) {
			return false;
		}
		return true;
	}

	void EventDispatcher::removeEventListener(std::string type) {
		eventListeners.erase(type);
	}

}