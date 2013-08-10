#include "AnimationState.h"
#include "../objects/Frame.h"
#include "../objects/AnimationData.h"
#include "../Armature.h"
#include "Animation.h"
#include "TimelineState.h"
#include <math.h>
#include "../Bone.h"
#include "../events/AnimationEvent.h"

namespace DragonBones {

	std::vector<AnimationState*> AnimationState::_pool = std::vector<AnimationState*>(0);

	void AnimationState::returnObject(AnimationState* animationState)
	{
		animationState->clear();
		bool found = false;
		for(int i=0;i<_pool.size();i++)
		{
			AnimationState* find = _pool[i];
			if(animationState == find)
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			_pool.push_back(animationState);
		}
	}
	
	TimelineState* AnimationState::getTimelineState(std::string value){
		if(_timelineStates.find(value) != _timelineStates.end())
			return _timelineStates.find(value)->second;
		return 0;
	}
	
	AnimationState* AnimationState::borrowObject(){
		if(_pool.size() == 0)
		{
			return new AnimationState();
		}
		AnimationState* pop = _pool[_pool.size()-1];
		_pool.pop_back();
		return pop;
	}
	
	std::string AnimationState::getName(){
		return _name;
	}
	
	AnimationData* AnimationState::getClip(){
		return _clip;
	}
	
	int AnimationState::getLoopCount(){
		return _loopCount;
	}
	
	int AnimationState::getLoop(){
		return _loop;
	}
	
	int AnimationState::getLayer(){
		return _layer;
	}
	
	bool AnimationState::isPlaying(){
		return _isPlaying && !_isComplete;
	}
	
	bool AnimationState::isComplete(){
		return _isComplete;
	}
	
	float AnimationState::getFadeInTime(){
		return _fadeInTime;
	}
	
	float AnimationState::getTotalTime(){
		return _totalTime;
	}
			
	void AnimationState::setCurrentTime(float value){
		if(value < 0 || !value)
			value = 0;
		_currentTime = value;
	}
	
	void AnimationState::setTimeScale(float value){
		if(value < 0)
		{
			value = 0;
		}
		else if(value)
		{
			value = 1;
		}
		//else if(std::isinf(_timeScale))
		//{
		//	_timeScale = 1;
		//}
		_timeScale = value;
	}

	bool AnimationState::getDisplayControl() {
		return _displayControl;
	}

	void AnimationState::setDisplayControl(bool value) {
		if(_displayControl != value)
		{
			_displayControl = value;
			if(_displayControl)
			{
				_armature->getAnimation()->setStatesDisplayControl(this);
			}
		}
	}
		
	AnimationState::AnimationState(){
		_timelineStates.clear();
	}

	AnimationState::~AnimationState(){
	}
	
	void AnimationState::fadeIn(Armature* armature, AnimationData* clip, float fadeInTime, float timeScale, int loop, int layer, bool displayControl, bool pauseBeforeFadeInComplete){
		_armature = armature;
		_clip = clip;
		_name = _clip->name;
		_layer = layer;
			
		_totalTime = _clip->getDuration();
		if(ceil((_clip->getDuration() * _clip->framerate)) < 2 || timeScale != timeScale) //CEIL rounds to highest number
		{
			
			_timeScale = 1;
			_currentTime = _totalTime;
			if(_loop >= 0)
			{
				_loop = 1;
			}
			else
			{
				_loop = -1;
			}
			_pauseBeforeFadeInComplete = false;
		}
		else
		{
			_timeScale = timeScale;
			_currentTime = 0;
			_loop = loop;
			_pauseBeforeFadeInComplete = pauseBeforeFadeInComplete;
		}

			
		_fadeInTime = fadeInTime * _timeScale;
			
			
		_loopCount = -1;
		_fadeState = 1;
		_fadeOutBeginTime = 0;
		_fadeOutWeight = _Nan._Float;
		_fadeWeight = 0;
		_displayControl = displayControl;
		_isPlaying = true;
		_isComplete = false;
		_fadeIn = true;
		_fadeOut = false;
			
		weight = 1;
		blend = true;
		enabled = true;
		tweenEnabled = true;
			
		updateTimelineStates();
	}
		
	void AnimationState::fadeOut(float fadeOutTime, bool pause){
		if(_fadeOutWeight != _fadeOutWeight) //NAN
		{
			return;
		}
		_fadeState = -1;
		_fadeOutWeight = _fadeWeight;
		_fadeOutTime = fadeOutTime * _timeScale;
		_fadeOutBeginTime = _currentTime;
		_isPlaying = !pause;
		_fadeOut = true;
		_displayControl = false;
			
		for (std::map<std::string, TimelineState*>::iterator it=_timelineStates.begin(); it != _timelineStates.end();it++)
		{
			TimelineState* timelineState = it->second;
			if(timelineState)
				timelineState->fadeOut();
		}
			
		enabled = true;
	}
		
	void AnimationState::play(){
		_isPlaying = true;
	}
	
	void AnimationState::stop(){
		_isPlaying = false;
	}
	
	int AnimationState::getMixingTransform(std::string timelineName){
		if(_mixingTransforms.size() > 0)
		{
			return _mixingTransforms[timelineName];
		}
		//if(_mixingTransforms.find(timelineName) != _mixingTransforms.end())
		//{
		//	return _mixingTransforms.find(timelineName)->second;
		//}
		return -1;
	}
	
	void AnimationState::addMixingTransform(std::string timelineName, int type, bool recursive){
		if(_clip && _clip->getTimeLine(timelineName))
		{
			if(recursive)
			{
				int i = _armature->_boneList.size();
				Bone* bone;
				Bone* currentBone;
				while(i > 0)
				{
					i--;
					bone = _armature->_boneList[i];
					if(bone->name == timelineName)
					{
						currentBone = bone;
					}
					if(currentBone && (currentBone == bone || currentBone->contains(bone)))
					{
						//_mixingTransforms.insert(std::make_pair(bone->name, type));
						_mixingTransforms[bone->name] = type;
					}
				}
			}
			else
			{
				//_mixingTransforms.insert(std::make_pair(timelineName, type));
				_mixingTransforms[timelineName] = type;
			}
				
			updateTimelineStates();
		}
	}
	
	void AnimationState::removeMixingTransform(std::string timelineName, bool recursive){
		if(timelineName != "")
		{
			if(recursive)
			{
				int i = _armature->_boneList.size();
				Bone* bone;
				Bone* currentBone;
				while(i > 0)
				{
					i--;
					bone = _armature->_boneList[i];
					if(bone->name == timelineName)
					{
						currentBone = bone;
					}
					if(currentBone && (currentBone == bone || currentBone->contains(bone)))
					{
					//	_mixingTransforms[bone->name] = -1;
						_mixingTransforms.erase(bone->name);
					}
				}
			}
			else
			{
				_mixingTransforms.erase(timelineName);
			}
			bool hasMixing = false;
			for (std::map<std::string, int>::iterator i=_mixingTransforms.begin(); i != _mixingTransforms.end();i++)
			{
				//std::string timelineName = i->first;
				hasMixing = true;
				break;
			}
			if(!hasMixing)
			{
				_mixingTransforms.clear();
			}
		}
		else
		{
			_mixingTransforms.clear();
		}
			
		updateTimelineStates();
	}

	bool AnimationState::advanceTime(float passedTime){
		if(!enabled)
		{
			return false;
		}
		AnimationEvent* aEvent = 0;	
		if(_fadeIn)
		{	
			_fadeIn = false;
			if(_armature->hasEventListener(AnimationEvent::FADE_IN))
			{
				aEvent = new AnimationEvent(AnimationEvent::FADE_IN);
				aEvent->animationState = this;
				_armature->dispatchEvent(aEvent);
				aEvent = 0;
			};
		}
			
		if(_fadeOut)
		{	
			_fadeOut = false;
			if(_armature->hasEventListener(AnimationEvent::FADE_OUT))
			{
				aEvent = new AnimationEvent(AnimationEvent::FADE_OUT);
				aEvent->animationState = this;
				_armature->dispatchEvent(aEvent);
				aEvent = 0;
			}
		}
			
		_currentTime += passedTime * _timeScale;
		
		if(_isPlaying && !_isComplete)
		{
			float progress;
			int currentLoopCount;
			if(_pauseBeforeFadeInComplete)
			{
				_pauseBeforeFadeInComplete = false;
				_isPlaying = false;
				progress = 0;
				currentLoopCount = progress;
			}
			else
			{
				progress = _currentTime / _totalTime;
					
				//update loopCount
				currentLoopCount = progress;
				if(currentLoopCount != _loopCount)
				{
					if(_loopCount == -1)
					{
						if(_armature->hasEventListener(AnimationEvent::START))
						{
							aEvent = new AnimationEvent(AnimationEvent::START);
							aEvent->animationState = this;
							_armature->dispatchEvent(aEvent);
							aEvent = 0;
						}
					}
					_loopCount = currentLoopCount;
					if(_loopCount)
					{
						if(_loop && _loopCount * _loopCount >= _loop * _loop - 1)
						{
							_isComplete = true;
							_isPlaying = false;
							progress = 1;
							currentLoopCount = 0;
							if(_armature->hasEventListener(AnimationEvent::COMPLETE))
							{
								aEvent = new AnimationEvent(AnimationEvent::COMPLETE);
								aEvent->animationState = this;
							}
						}
						else
						{
							if(_armature->hasEventListener(AnimationEvent::LOOP_COMPLETE))
							{
								aEvent = new AnimationEvent(AnimationEvent::LOOP_COMPLETE);
								aEvent->animationState = this;
							}
						}
					}
				}
			}
				
				
			for (std::map<std::string, TimelineState*>::iterator i=_timelineStates.begin(); i != _timelineStates.end();i++)
			{
				TimelineState* timeline = i->second;
				if(timeline) 
					timeline->update(progress);
			}
			
			
			if(_clip->getFrameList().size() > 0)
			{
				float playedTime = _totalTime * (progress - currentLoopCount);
				bool isArrivedFrame = false;
				int frameIndex = 0;
				_currentFrame = _clip->getFrameList()[0];
				while(!_currentFrame || playedTime > _currentFrame->position + _currentFrame->duration || playedTime < _currentFrame->position)
				{

					if(isArrivedFrame)
					{
						_armature->arriveAtFrame(_currentFrame, 0, this, true);
					}
					isArrivedFrame = true;
					if(_currentFrame)
					{
						for(int i =0; i<_clip->getFrameList().size();i++) {
							Frame* found = _clip->getFrameList()[i];
							if(found==_currentFrame) {
								frameIndex=i;
							}
						}

						frameIndex ++;
						if(frameIndex >= _clip->getFrameList().size())
						{
							frameIndex = 0;
						}
						_currentFrame = _clip->getFrameList()[frameIndex];
					}
					else
					{
						_currentFrame = _clip->getFrameList()[0];
					}

				}
					
				if(isArrivedFrame)
				{
					_armature->arriveAtFrame(_currentFrame, 0, this, false);
				}
			}

				
			if(aEvent)
			{
				_armature->dispatchEvent(aEvent);
			}
		}
			
			//update weight and fadeState
		if(_fadeState > 0)
		{
			if(_fadeInTime == 0)
			{
				_fadeWeight = 1;
				_fadeState = 0;
				_isPlaying = true;
				if(_armature->hasEventListener(AnimationEvent::FADE_IN_COMPLETE))
				{
					aEvent = new AnimationEvent(AnimationEvent::FADE_IN_COMPLETE);
					aEvent->animationState = this;
					_armature->dispatchEvent(aEvent);
				}
			}
			else
			{
				_fadeWeight = _currentTime / _fadeInTime;
				if(_fadeWeight >= 1)
				{
					_fadeWeight = 1;
					_fadeState = 0;
					if(!_isPlaying)
					{
						_currentTime -= _fadeInTime;
					}
					_isPlaying = true;
					if(_armature->hasEventListener(AnimationEvent::FADE_IN_COMPLETE))
					{
						aEvent = new AnimationEvent(AnimationEvent::FADE_IN_COMPLETE);
						aEvent->animationState = this;
						_armature->dispatchEvent(aEvent);
					}
				}
			}
		}
		else if(_fadeState < 0)
		{
			if(_fadeOutTime == 0)
			{
				_fadeWeight = 0;
				_fadeState = 0;
				if(_armature->hasEventListener(AnimationEvent::FADE_OUT_COMPLETE))
				{
					aEvent = new AnimationEvent(AnimationEvent::FADE_OUT_COMPLETE);
					aEvent->animationState = this;
					_armature->dispatchEvent(aEvent);
				}
				return true;
			}
			else
			{
				_fadeWeight = (1 - (_currentTime - _fadeOutBeginTime) / _fadeOutTime) * _fadeOutWeight;
				if(_fadeWeight <= 0)
				{
					_fadeWeight = 0;
					_fadeState = 0;
					if(_armature->hasEventListener(AnimationEvent::FADE_OUT_COMPLETE))
					{
						aEvent = new AnimationEvent(AnimationEvent::FADE_OUT_COMPLETE);
						aEvent->animationState = this;
						_armature->dispatchEvent(aEvent);
					}
					return true;
				}
			}
		}
			
		if(_isComplete && _loop < 0)
		{
			//fadeOut(_fadeInTime, true);

			//fadeOut((_fadeOutWeight || _fadeInTime) / _timeScale, true);
			fadeOut( _fadeInTime / _timeScale, true);
		}

		return false;
	}

	void AnimationState::updateTimelineStates(){
		if(_mixingTransforms.size() > 0)
		{
			for (std::map<std::string, TimelineState*>::iterator i=_timelineStates.begin(); i != _timelineStates.end();i++)
			{
				std::string timelineName = i->first;
				if(_mixingTransforms[timelineName] == 0)
				{
					removeTimelineState(timelineName);

				}
			}
				
			for (std::map<std::string, int>::iterator i=_mixingTransforms.begin(); i != _mixingTransforms.end();i++)
			{
				std::string timelineName = i->first;
				if(!_timelineStates[timelineName])
				{
					addTimelineState(timelineName);
				}
			}
		}
		else
		{
			for (std::map<std::string, TransformTimeline*>::iterator i=_clip->_timeslines.begin(); i != _clip->_timeslines.end();i++)
			{
				std::string timelineName = i->first;
				if(!_timelineStates[timelineName])
				{
					addTimelineState(timelineName);
				}
			}
		}
	}

	void AnimationState::addTimelineState(std::string timelineName){
		Bone* bone = _armature->getBone(timelineName);
		if(bone)
		{
			TimelineState* timelineState = TimelineState::borrowObject();
			TransformTimeline* timeline = _clip->getTimeLine(timelineName);
			timelineState->fadeIn(bone, this, timeline);
			_timelineStates[timelineName] = timelineState;
			//_timelineStates.insert(std::make_pair(timelineName, timelineState));
		}
	}

	void AnimationState::removeTimelineState(std::string timelineName){
		//if(timelineName != "")
		//{
			//if(_timelineStates.find(timelineName) != _timelineStates.end())
			//{
				TimelineState::returnObject(_timelineStates[timelineName]);
				//_timelineStates.erase(timelineName);
				delete _timelineStates[timelineName];
			//}
		//}
	}

	void AnimationState::clear(){
		_armature = 0;
		_currentFrame = 0;
		_clip = 0;
		_mixingTransforms.clear();
		enabled = false;
		
			
		for (std::map<std::string, TimelineState*>::iterator i=_timelineStates.begin(); i != _timelineStates.end(); i++)
		{
			std::string timelineName = i->first;
			//removeTimelineState(timelineName);
			//TimelineState::returnObject(i->second);
			TimelineState::returnObject(_timelineStates[timelineName]);
			//_timelineStates[timelineName] = new TimelineState();
			//_timelineStates.erase(timelineName);
		}

		_timelineStates.clear();

	}

	void AnimationState::clearStatic() {
		int i = _pool.size();
		while(i > 0)
		{
			i--;
			_pool[i]->clear();
		}
		_pool.clear();
			
		TimelineState::clearStatic();
	}
}
