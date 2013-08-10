#ifndef DRAGONBONES_FRAMEEVENT_H
#define DRAGONBONES_FRAMEEVENT_H

#include <string>
#include "../as3/Event.h"

namespace DragonBones {
	class Armature;
	class Bone;
	class AnimationState;
	class FrameEvent : public Event {
	public:
		static const std::string MOVEMENT_FRAME_EVENT;
		static const std::string ANIMATION_FRAME_EVENT;
		static const std::string BONE_FRAME_EVENT;

		std::string frameLabel;
		Bone* bone;
		Armature* getArmature();

		AnimationState* animationState;
		FrameEvent(std::string type);
		Event* clone();

	};
}

#endif