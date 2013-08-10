#include "AnimationEvent.h"
#include "../Armature.h"
#include "../animation/AnimationState.h"

namespace DragonBones {
	const std::string AnimationEvent::MOVEMENT_CHANGE = "fadeIn";
	const std::string AnimationEvent::FADE_IN = "fadeIn";
	const std::string AnimationEvent::FADE_OUT = "fadeOut";
	const std::string AnimationEvent::START = "start";
	const std::string AnimationEvent::COMPLETE = "complete";
	const std::string AnimationEvent::LOOP_COMPLETE = "loopComplete";
	const std::string AnimationEvent::FADE_IN_COMPLETE = "fadeInComplete";
	const std::string AnimationEvent::FADE_OUT_COMPLETE = "fadeOutComplete";

	Armature* AnimationEvent::getArmature() {
		return (Armature*) target;
	}

	std::string AnimationEvent::getMovementID(){
		return animationState->getName();
	}

	AnimationEvent::AnimationEvent(std::string type) : Event(type){
	}

	Event* AnimationEvent::clone(){
		AnimationEvent* eve = new AnimationEvent(type);
		eve->animationState = animationState;
		return eve;
	}
}