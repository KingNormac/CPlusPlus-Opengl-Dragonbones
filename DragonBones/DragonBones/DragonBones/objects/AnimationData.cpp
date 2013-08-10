#include "AnimationData.h"

namespace DragonBones {

	AnimationData::AnimationData() : Timeline() {
		loop = 1;
		tweenEasing = _Nan._Float;
		_fadeTime = -1;
		name = "";
		framerate = 0;
	}

	AnimationData::~AnimationData() {
		typedef std::map<std::string, TransformTimeline*>::iterator it_type;
		for(it_type iterator = _timeslines.begin(); iterator != _timeslines.end(); iterator++) {
			TransformTimeline* timeline = iterator->second;
			if(timeline)
				timeline->dispose();
		}
		_timeslines.clear();
	}

	float AnimationData::getFadeInTime() {
		return this->_fadeTime;
	}

	void AnimationData::setFadeInTime(float value) {
		if(value > 0)
			this->_fadeTime = value;
		else
			this->_fadeTime = 0;
	}

	TransformTimeline* AnimationData::getTimeLine(std::string name) {
		//typedef std::map<std::string, TransformTimeline*>::iterator it_type;
		//it_type itr = _timeslines.find(name);
		//if(itr != _timeslines.end()) {
		//	TransformTimeline* timeline = itr->second;
		//	if(timeline)
				//return timeline;
		//}
		return _timeslines[name];
		//return 0;
	}

	void AnimationData::addTimeLine(TransformTimeline* timeline, std::string timelineName) {
		if(timeline) {
			//_timeslines.insert(std::make_pair(timelineName, timeline));
			_timeslines[timelineName] = timeline;
		} else {
			printf("Tried to insert bad timeline into AnimationData \n");
		}
	}
};