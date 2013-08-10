#ifndef DRAGONBONE_ANIMATIONEVENT_H
#define DRAGONBONE_ANIMATIONEVENT_H

#include <string>
#include "../as3/Event.h"

namespace DragonBones {
	class AnimationState;
	class Armature;
	class AnimationEvent : public Event {
	public:
		static const std::string MOVEMENT_CHANGE;
		static const std::string FADE_IN;
		static const std::string FADE_OUT;
		static const std::string START;
		static const std::string COMPLETE;
		static const std::string LOOP_COMPLETE;
		static const std::string FADE_IN_COMPLETE;
		static const std::string FADE_OUT_COMPLETE;


		AnimationState* animationState;

		Armature* getArmature();
		std::string getMovementID();
		AnimationEvent(std::string type);
		Event* clone();

	};
}

#endif