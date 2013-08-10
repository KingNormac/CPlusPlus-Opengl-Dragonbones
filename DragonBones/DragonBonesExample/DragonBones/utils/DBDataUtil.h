#ifndef DBDATAUTIL_H
#define DBDATAUTIL_H

#include "../objects/ArmatureData.h"


namespace DragonBones {
	namespace DBDataUtil {

		void transformArmatureData(ArmatureData* armatureData);
		void transformArmatureDataAnimations(ArmatureData* armatureData);
		void transformAnimationData(AnimationData* animationData, ArmatureData* armatureData);
		void getTimelineTransform(TransformTimeline* timeline, float position, DBTransform* result);
		void addHideTimeline(AnimationData* animationData, ArmatureData* armatureData);

	};
};


#endif