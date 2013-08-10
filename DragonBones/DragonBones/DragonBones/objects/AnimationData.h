#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H

#include "TransformTimeline.h"
#include <map>

namespace DragonBones {

	class AnimationData : public Timeline {
	private:
		
		float _fadeTime;
	public:
		int framerate;
		std::string name;
		int loop;
		float tweenEasing;
		std::map<std::string, TransformTimeline*> _timeslines;
		AnimationData();
		~AnimationData();

		float getFadeInTime();
		void setFadeInTime(float value);

		TransformTimeline* getTimeLine(std::string name);
		void addTimeLine(TransformTimeline* timeline, std::string timelineName);

		

	};

};



#endif