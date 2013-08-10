#include "TimeLine.h"

namespace DragonBones {

	Timeline::Timeline() : Object("Timeline"){
		//_duration = 0;
		//_scale = 1;
	}

	Timeline::~Timeline() {
		for(int i=0; i<_frameList.size();i++) {
			Frame* frame = _frameList[i];
			if(frame) {
				delete[] frame;
			}
		}
		_frameList.clear();
	}
	
	std::vector<Frame*> Timeline::getFrameList() {
		return this->_frameList;
	}

	void Timeline::addFrame(Frame* frame) {
		if(!frame) {
			printf("Try added a frame that is not a frame \n");
		} else if(frame) {
			_frameList.push_back(frame);
		}
	}

	float Timeline::getDuration() {
		return this->_duration;
	}

	void Timeline::setDuration(float value) {
		this->_duration = value;
	}

	float Timeline::getScale() {
		return this->_scale;
	}

	void Timeline::setScale(float value) {
		this->_scale = value;
	}
};
