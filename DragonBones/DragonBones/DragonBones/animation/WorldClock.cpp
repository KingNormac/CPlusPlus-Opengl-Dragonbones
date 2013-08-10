#include "WorldClock.h"
#include <algorithm>

namespace DragonBones {

	Clock* Clock::Instance()
	{
		static Clock instance;
		return &instance;
	}

	Clock::Clock() {
		_time = getTimer() * 0.001;
		timeScale = 1;
	}

	double Clock::getTime() {
		return _time;
	}

	bool Clock::contains(IAnimatable* animatable) {
		for(int i=0;i<this->animatableList.size();i++) {
			IAnimatable* found = animatableList[i];
			if(found)
				if(found == animatable)
					return true;
		}
		return false;
	}

	void Clock::add(IAnimatable* animatable) {
		for(int i=0;i<this->animatableList.size();i++) {
			IAnimatable* found = animatableList[i];
			if(found)
				if(found == animatable)
					return;	
		}
		animatableList.push_back(animatable);
	}

	void Clock::remove(IAnimatable* animatable) {
		if(animatable)
			animatableList.erase(std::remove(animatableList.begin(), animatableList.end(), animatable), animatableList.end());
	}

	void Clock::clear() {
		animatableList.clear();
	}

	void Clock::advanceTime(float passedTime) {
		if(passedTime < 0) {
			float currentTime = getTimer() * 0.001;
			passedTime = currentTime - _time;
			_time = currentTime;
		}
			
		passedTime *= timeScale;
			
		int length = animatableList.size();
		if(length == 0) {
			return;
		}

		for(IAnimatable* animatable : animatableList) {
			if(animatable){
				animatable->advanceTime(passedTime);
			}
		}
	}
}