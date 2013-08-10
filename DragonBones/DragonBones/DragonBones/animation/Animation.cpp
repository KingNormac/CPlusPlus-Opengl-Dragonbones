#include "Animation.h"
#include "../Bone.h"
#include "TimelineState.h"
#include "../objects/AnimationData.h"

namespace DragonBones {
	const std::string Animation::FADE_OUT_NONE = "fadeOutNone";
	const std::string Animation::FADE_OUT_SAME_LAYER = "fadeOutSameLayer";
	const std::string Animation::FADE_OUT_ALL_LAYER = "fadeOutAllLayer";

	void Animation::setStatesDisplayControl(AnimationState* animationState){
		int i = _animationLayer.size();
		std::vector<AnimationState*> animationStateList = _animationLayer[i];
		int j;
		while(i > 0)
		{
			i--;
			j = _animationLayer[i].size();
			while(j > 0)
			{
				j--;
				_animationLayer[i][j]->setDisplayControl(_animationLayer[i][j] == animationState);
			}
		}
	}

	int Animation::addLayer(int layer){
		if(layer >= _animationLayer.size())
		{
			layer = _animationLayer.size();
			_animationLayer.push_back(std::vector<AnimationState*>());
		}
		return layer;
	}

	void Animation::addState(AnimationState* animationState){
		_animationLayer[animationState->getLayer()].push_back(animationState);
	}

	void Animation::removeState(AnimationState* animationState){
		int layer = animationState->getLayer();

		for(int i=0;i<_animationLayer[layer].size();i++)
		{
			AnimationState* found = _animationLayer[layer][i];
			if(found==animationState) {
				//_animationLayer[layer].erase(_animationLayer[layer].begin() + i);
				_animationLayer[layer].erase(_animationLayer[layer].begin() + i);
			}
		}

		AnimationState::returnObject(animationState);

		if(_animationLayer[layer].size() == 0 && layer == _animationLayer.size() - 1)
		{
			_animationLayer.resize(_animationLayer.size()-1);
		}
	}

	std::vector<std::string> Animation::getMovementList(){
		return _animationList;
	}

	std::string Animation::getMovementID(){
		if(_lastAnimationState)
			return _lastAnimationState->getName();
		return "";
	}

	AnimationState* Animation::getLastAnimationState(){
		return _lastAnimationState;
	}

	std::vector<std::string> Animation::getAnimationList(){
		return _animationList;
	}

	bool Animation::isPlaying(){
		return _isPlaying && !isComplete();
	}

	bool Animation::isComplete(){
		if(_lastAnimationState)
		{
			if(!_lastAnimationState->isComplete())
			{
				return false;
			}
			int j = _animationLayer.size();
			while(j > 0)
			{
				j--;
				std::vector<AnimationState*> animationStateList = _animationLayer[j];
				int i = animationStateList.size();
				while(i > 0)
				{
					i--;
					if(!animationStateList[i]->isComplete())
					{
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	std::vector<AnimationData*> Animation::getAnimationDataList(){
		return _animationDataList;
	}

	void Animation::setAnimationDataList(std::vector<AnimationData*> value){
		_animationDataList = value;
		_animationList.clear();
		for(AnimationData* animationData : _animationDataList)
		{
			_animationList.push_back(animationData->name);
		}
	}

	float Animation::getTimeScale(){
		return _timeScale;
	}

	void Animation::setTimeScale(float value){
		if (value < 0)
		{
			value = 0;
		}
		_timeScale = value;
	}

	Animation::Animation(Armature* armature){
		_armature = armature;
		_animationLayer = std::vector<std::vector<AnimationState*>>();
		_animationList = std::vector<std::string>();
		_lastAnimationState = 0;
			
		this->_isPlaying = false;
		//this->isComplete = false;
		_timeScale = 1;
		tweenEnabled = true;
	}

	Animation::~Animation(){
		stop();
		int i = _animationLayer.size();
		while(i > 0)
		{
			i--;
			std::vector<AnimationState*> animationStateList = _animationLayer[i];
			int j = animationStateList.size();
			while(j > 0)
			{
				j--;
				AnimationState::returnObject(_animationLayer[i][j]);
			}
			animationStateList.clear();
		}
		_animationLayer.clear();
		_animationList.clear();
			
		_armature = 0;
	}

	AnimationState* Animation::gotoAndPlay(std::string animationName, float fadeInTime, float duration, float loop, int layer, 
		std::string playMode,bool displayControl,bool pauseFadeOut,bool pauseFadeIn)
	{
		if (_animationDataList.size() <= 0)
		{
			return 0;
		}
		int i = _animationDataList.size();
		AnimationData* animationData = 0;
		while(i > 0)
		{
			i--;
			if(_animationDataList[i]->name == animationName)
			{
				animationData = _animationDataList[i];
				break;
			}
		}
		if (!animationData)
		{
			return 0;
		}
			
		_isPlaying = true;
			
		if(fadeInTime < 0) {
			if(animationData->getFadeInTime() < 0) {
				animationData->setFadeInTime(0.3f);
			} else {
				//animationData->setFadeInTime(animationData->getFadeInTime());
			}
			fadeInTime = animationData->getFadeInTime();
		} else {
			fadeInTime = fadeInTime;
		}
		
		float durationScale;
		if(duration < 0)
		{
			if(animationData->getScale() < 0)
				animationData->setScale(1);
			else
				animationData->setScale(animationData->getScale());
			durationScale = animationData->getScale();
		}
		else
		{
			durationScale = duration / animationData->getDuration();
		}

		if(loop == _Nan._Float)
		{
			loop = animationData->loop;
		}
			
		layer = addLayer(layer);
			
		//autoSync = autoSync && !pauseFadeOut && !pauseFadeIn;
		std::string group = "";
		if(playMode == FADE_OUT_NONE)
		{
			//DO NOTHING;
		} else if(playMode == FADE_OUT_SAME_LAYER)
		{
			std::vector<AnimationState*> animationStateList = _animationLayer[layer];
			i = animationStateList.size();
			while(i > 0)
			{
				i--;
				AnimationState* animationState = animationStateList[i];
				if(animationState->group == "")
				{
					animationState->fadeOut(fadeInTime, pauseFadeOut);
				}
			}
		} else if(playMode == FADE_OUT_ALL_LAYER)
		{
			int j = _animationLayer.size();
			
			while(j > 0)
			{
				j--;
				std::vector<AnimationState*> animationStateList = _animationLayer[layer];
				int i = animationStateList.size();
				while(i > 0)
				{
					i--;
					AnimationState* animationState = animationStateList[i];
					if(animationState->group == "")
					{
						animationState->fadeOut(fadeInTime, pauseFadeOut);
					}
				}
			}
		} else { //DEFAULT
			group = playMode;
			int j = _animationLayer.size();
			while(j > 0)
			{
				j--;
				std::vector<AnimationState*> animationStateList = _animationLayer[j];
				i = animationStateList.size();
				while(i > 0)
				{
					i--;
					AnimationState* animationState = animationStateList[i];
					if(animationState->group == group)
					{
						animationState->fadeOut(fadeInTime, pauseFadeOut);
					}
				}
			}
		}

		std::vector<Bone*> boneList = _armature->_boneList;
		Bone* bone;
		int v = boneList.size();
		while(v > 0)
		{
			v--;
			bone = boneList[v];
			if(bone->getChildArmature())
			{
				bone->getChildArmature()->getAnimation()->gotoAndPlay(animationName);
			}
		}

		_lastAnimationState = AnimationState::borrowObject();
		_lastAnimationState->group = group;
		_lastAnimationState->tweenEnabled = tweenEnabled;
		_lastAnimationState->fadeIn(_armature, animationData, fadeInTime, 1 / durationScale, loop, layer, displayControl, pauseFadeIn);
			
		addState(_lastAnimationState);
		return _lastAnimationState;
	}

	void Animation::play(){
		if ( _animationDataList.size() == 0)
		{
			return;
		}
		if(!_lastAnimationState)
		{
			gotoAndPlay(_animationDataList[0]->name);
		}
		else if (!_isPlaying)
		{
			_isPlaying = true;
		}
		else
		{
			gotoAndPlay(_lastAnimationState->getName());
		}

	}

	void Animation::stop(){
		_isPlaying = false;
	}

	AnimationState* Animation::getState(std::string name, int layer){
		int l = _animationLayer.size();
		if(l == 0)
		{
			return 0;
		}
		else if(layer >= l)
		{
			layer = l - 1;
		}
		
		std::vector<AnimationState*> animationStateList = _animationLayer[layer];
		if(animationStateList.size() == 0)
		{
			return 0;
		}
		int i = animationStateList.size();
		while(i > 0)
		{
			i--;
			if(animationStateList[i]->getName() == name)
			{
				return animationStateList[i];
			}
		}
		return 0;
	}

	void Animation::advanceTime(float passedTime) {
		if(!_isPlaying)
		{
			return;
		}
		passedTime *= _timeScale;

		
		int l = _armature->_boneList.size();
		int i;
		int j;
		int k = l;
		Bone* bone = 0;
		std::string boneName;
		float weigthLeft;
		
		float x=0;
		float y=0;
		float skewX=0;
		float skewY=0;
		float scaleX=0;
		float scaleY=0;
		float pivotX=0;
		float pivotY=0;

		float layerTotalWeight = 0;
		std::vector<AnimationState*> animationStateList;
		AnimationState* animationState = 0;
		TimelineState* timelineState = 0;
		float weight = 1;
		DBTransform* transform = 0;
		Vector2D* pivot = 0;
		l--;
		while(k > 0)
		{
			k--;
			bone = _armature->_boneList[k];
			boneName = bone->name;
			weigthLeft = 1;
				
			x = 0;
			y = 0;
			skewX = 0;
			skewY = 0;
			scaleX = 0;
			scaleY = 0;
			pivotX = 0;
			pivotY = 0;
				
			i = _animationLayer.size();
			while(i > 0)
			{
				i--;
				layerTotalWeight = 0;
				animationStateList = _animationLayer[i];
				j = animationStateList.size();
				while(j > 0)
				{
					j--;
					animationState = animationStateList[j];
					if(k == l)
					{
						if(animationState->advanceTime(passedTime))
						{
							removeState(animationState);
							continue;
						}
					}
						
					//timelineState = animationState->getTimelineState(boneName);
					timelineState = animationState->_timelineStates[boneName];
						
					if(timelineState)
					{
						weight = animationState->_fadeWeight * animationState->weight * weigthLeft;
						transform = timelineState->transform;
						pivot = timelineState->pivot;
						x += transform->x * weight;
						y += transform->y * weight;
						skewX += transform->skewX * weight;
						skewY += transform->skewY * weight;
						scaleX += transform->scaleX * weight;
						scaleY += transform->scaleY * weight;
						pivotX += pivot->x * weight;
						pivotY += pivot->y * weight;
							
						layerTotalWeight += weight;
					}
				}

					
				if(layerTotalWeight >= weigthLeft)
				{
					break;
				}
				else
				{
					weigthLeft -= layerTotalWeight;
				}
			}

			transform = bone->_tween;
			pivot = bone->_tweenPivot;
				
			transform->x = x;
			transform->y = y;
			transform->skewX = skewX;
			transform->skewY = skewY;
			transform->scaleX = scaleX;
			transform->scaleY = scaleY;
			pivot->x = pivotX;
			pivot->y = pivotY;
		}
	}
}