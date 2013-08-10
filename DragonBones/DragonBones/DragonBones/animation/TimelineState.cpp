#include "TimelineState.h"
#include <math.h>
#include "AnimationState.h"
#include "Animation.h"
#include "../utils/TransformUtil.h"
#include "../objects/AnimationData.h"
#include "../Bone.h"
#include "../objects/TransformTimeline.h"
#include "../objects/TransformFrame.h"
#include "../as3/ColorTransform.h"
#include "../objects/DBTransform.h"
#include "../as3/Vector2D.h"

namespace DragonBones {
	const double TimelineState::M_PI = atan(1)*4;
	const double TimelineState::HALF_PI = atan(1)*4 * 0.5;
	const double TimelineState::DOUBLE_PI = atan(1)*4* 2;

	std::vector<TimelineState*> TimelineState::_pool = std::vector<TimelineState*>();

	float TimelineState::getEaseValue(float value, float easing) {
		float valueEase;
		if (easing > 1)
		{
			valueEase = 0.5 * (1 - cos(value * M_PI )) - value;
			easing -= 1;
		}
		else if (easing > 0)
		{
			valueEase = sin(value * HALF_PI) - value;
		}
		else if (easing < 0)
		{
			valueEase = 1 - cos(value * HALF_PI) - value;
			easing *= -1;
		}
		return valueEase * easing + value;
	}

	TimelineState* TimelineState::borrowObject(){
		if(_pool.size() == 0)
		{
			return new TimelineState();
		}
		TimelineState* pop = _pool[_pool.size()-1];
		_pool.pop_back();
		return pop;
	}

	void TimelineState::returnObject(TimelineState* timeline){
		bool found = false;

		for(TimelineState* find : _pool)
		{
			if(timeline == find) {
				found = true;
				break;
			}
		}

		if(!found)
		{
			_pool.push_back(timeline);
			//_pool.insert(_pool.begin()+_pool.size(), timeline);
			//_pool[_pool.size()] = timeline;
		}
			
		timeline->clear();
	}

	void TimelineState::clearStatic(){
		int i = _pool.size();
		while(i > 0)
		{
			i--;
			_pool[i]->clear();
		}
		_pool.clear();
	}

	TimelineState::TimelineState(){
		transform = new DBTransform();
		pivot = new Vector2D();

		_currentFrame = 0;

			
		//_originTransform = new DBTransform();

		_currentFrameDuration = 0;
		_currentFramePosition = 0;
			
		_durationTransform = new DBTransform();
		_durationPivot = new Vector2D();
		_durationColor = new ColorTransform();
	}

	TimelineState::~TimelineState(){
	}

	void TimelineState::fadeIn(Bone* bone, AnimationState* animationState, TransformTimeline* timeline){
		_bone = bone;
		_animationState = animationState;
		_timeline = timeline;
			
		_originTransform = _timeline->originTransform;
		_originPivot = _timeline->originPivot;
			//_originTransform.copy(_timeline.originTransform);
			
			/*
			var bLRX:Number = _bone.origin.skewX + _bone.offset.skewX + _bone._tween.skewX;
			var bLRY:Number = _bone.origin.skewY + _bone.offset.skewY + _bone._tween.skewY;
			
			_originTransform.skewX = bLRX + TransformUtils.formatRadian(_originTransform.skewX - bLRX);
			_originTransform.skewY = bLRY + TransformUtils.formatRadian(_originTransform.skewY - bLRY);
			*/
			
		_totalTime = _animationState->getTotalTime();
			
		transform->x = 0;
		transform->y = 0;
		transform->scaleX = 1;
		transform->scaleY = 1;
		transform->skewX = 0;
		transform->skewY = 0;
		pivot->x = 0;
		pivot->y = 0;
			
		_durationTransform->x = 0;
		_durationTransform->y = 0;
		_durationTransform->scaleX = 1;
		_durationTransform->scaleY = 1;
		_durationTransform->skewX = 0;
		_durationTransform->skewY = 0;
		_durationPivot->x = 0;
		_durationPivot->y = 0;
		switch(_timeline->_frameList.size())
		{
			case 0:
				_bone->arriveAtFrame(0, this, _animationState, false);
				updatef = &TimelineState::updateNothing;
				break;
			case 1:
				updatef = &TimelineState::updateSingle;
				break;
			default:
				updatef = &TimelineState::updateList;
				break;
		}
	}

	void TimelineState::fadeOut(){
		transform->skewX = TranformUtils::formatRadian(transform->skewX);
		transform->skewY = TranformUtils::formatRadian(transform->skewY);
		//_originTransform.skewX = TransformUtil.formatRadian(_originTransform.skewX);
		//_originTransform.skewY = TransformUtil.formatRadian(_originTransform.skewY);
	}

	int TimelineState::updateNothing(float progress){
		return 0;
	}

	int TimelineState::updateSingle(float progress){
		updatef = &TimelineState::updateNothing;
			
		if(_animationState->blend)
		{
			transform->copy(_originTransform);
				
			pivot->x = _originPivot->x;
			pivot->y = _originPivot->y;
		}
		else
		{
				transform->x = 0;
				transform->y = 0;
				transform->skewX = 0;
				transform->skewY = 0;
				transform->scaleX = 0;
				transform->scaleY = 0;
				
			pivot->x = 0;
			pivot->y = 0;
		}
			
		_currentFrame = (TransformFrame*)_timeline->_frameList[0];
			
		if(_currentFrame->color)
		{
			_bone->updateColor(
				_currentFrame->color->alphaOffset, 
				_currentFrame->color->redOffset, 
				_currentFrame->color->greenOffset, 
				_currentFrame->color->blueOffset, 
				_currentFrame->color->alphaMultiplier, 
				_currentFrame->color->redMultiplier, 
				_currentFrame->color->greenMultiplier, 
				_currentFrame->color->blueMultiplier,
				true
			);
		}
		else
		{
			_bone->updateColor(0, 0, 0, 0, 1, 1, 1, 1, false);
		}
			
		_bone->arriveAtFrame(_currentFrame, this, _animationState, false);
		return 1;
	}

	int TimelineState::updateList(float progress){
		if(_timeline->getScale() == 0)
		{
			progress = 1;
		}
		else
		{
			progress /= _timeline->getScale();
		}
		
		if(progress == 1)
		{
			progress = 0.99999999;

		}
			
		progress += _timeline->getOffset();
		int loopCount = progress;
		progress -= loopCount;
		float playedTime = _totalTime * progress;
		bool isArrivedFrame = false;
		int frameIndex = 0;
		while (!_currentFrame || playedTime > _currentFramePosition + _currentFrameDuration || playedTime < _currentFramePosition)
		{
			if(isArrivedFrame)
			{
				_bone->arriveAtFrame(_currentFrame, this, _animationState, true);
			}
			isArrivedFrame = true;
			if(_currentFrame)
			{
				for(int i=0;i<_timeline->_frameList.size();i++) {
					Frame* frame = _timeline->_frameList[i];
					if(frame==_currentFrame) {
						frameIndex = i;
						break;
					}
				}
				frameIndex++;
				if(frameIndex >= _timeline->_frameList.size())
				{
					frameIndex = 0;
				}
				_currentFrame = (TransformFrame*)_timeline->_frameList[frameIndex];
			}
			else
			{
				frameIndex = 0;
				_currentFrame = (TransformFrame*)_timeline->_frameList[0];
			}
			_currentFrameDuration = _currentFrame->duration;
			_currentFramePosition = _currentFrame->position;
		}

		if(isArrivedFrame)
		{
			frameIndex ++;
			if(frameIndex >= _timeline->_frameList.size())
			{
				frameIndex = 0;
			}
			TransformFrame* nextFrame = (TransformFrame*)_timeline->_frameList[frameIndex];
				
			if(frameIndex == 0 && _animationState->getLoop() && _animationState->getLoopCount() >= abs(_animationState->getLoop()) - 1 && ((_currentFramePosition + _currentFrameDuration) / _totalTime + loopCount - _timeline->getOffset()) * _timeline->getScale() > 0.999999)// >= 1
			{
				updatef = &TimelineState::updateNothing;
				_tweenEasing = _Nan._Float;
			}
			else if(nextFrame->displayIndex < 0 || !_animationState->tweenEnabled)
			{
				_tweenEasing = _Nan._Float;
			}
			else if(_animationState->getClip()->tweenEasing == _Nan._Float)
			{
				_tweenEasing = _currentFrame->tweenEasing;
			}
			else
			{
				_tweenEasing = _animationState->getClip()->tweenEasing;
			}
			
			if(_tweenEasing == _Nan._Float)
			{
				_tweenTransform = false;
				_tweenColor = false;
			}
			else
			{
				_durationTransform->x = nextFrame->transform->x - _currentFrame->transform->x;
				_durationTransform->y = nextFrame->transform->y - _currentFrame->transform->y;
				_durationTransform->skewX = nextFrame->transform->skewX - _currentFrame->transform->skewX;
				_durationTransform->skewY = nextFrame->transform->skewY - _currentFrame->transform->skewY;
				_durationTransform->scaleX = nextFrame->transform->scaleX - _currentFrame->transform->scaleX;
				_durationTransform->scaleY = nextFrame->transform->scaleY - _currentFrame->transform->scaleY;
					
				if(frameIndex == 0)
				{
					_durationTransform->skewX = TranformUtils::formatRadian(_durationTransform->skewX);
					_durationTransform->skewY = TranformUtils::formatRadian(_durationTransform->skewY);
				}
					
				_durationPivot->x = nextFrame->pivot->x - _currentFrame->pivot->x;
				_durationPivot->y = nextFrame->pivot->y - _currentFrame->pivot->y;
				
				if(
					(_durationTransform->x != 0) ||
					(_durationTransform->y != 0) ||
					(_durationTransform->skewX != 0) ||
					(_durationTransform->skewY != 0) ||
					(_durationTransform->scaleX != 0) ||
					(_durationTransform->scaleY != 0) ||
					(_durationPivot->x != 0 )||
					(_durationPivot->y != 0)
				)
				{
					_tweenTransform = true;
				}
				else
				{
					_tweenTransform = false;
				}
				
				if(_currentFrame->color && nextFrame->color)
				{
					_durationColor->alphaOffset = nextFrame->color->alphaOffset - _currentFrame->color->alphaOffset;
					_durationColor->redOffset = nextFrame->color->redOffset - _currentFrame->color->redOffset;
					_durationColor->greenOffset = nextFrame->color->greenOffset - _currentFrame->color->greenOffset;
					_durationColor->blueOffset = nextFrame->color->blueOffset - _currentFrame->color->blueOffset;
					
					_durationColor->alphaMultiplier = nextFrame->color->alphaMultiplier - _currentFrame->color->alphaMultiplier;
					_durationColor->redMultiplier = nextFrame->color->redMultiplier - _currentFrame->color->redMultiplier;
					_durationColor->greenMultiplier = nextFrame->color->greenMultiplier - _currentFrame->color->greenMultiplier;
					_durationColor->blueMultiplier = nextFrame->color->blueMultiplier - _currentFrame->color->blueMultiplier;
					
					if(
						_durationColor->alphaOffset != 0 ||
						_durationColor->redOffset != 0 ||
						_durationColor->greenOffset != 0 ||
						_durationColor->blueOffset != 0 ||
						_durationColor->alphaMultiplier != 0 ||
						_durationColor->redMultiplier != 0 ||
						_durationColor->greenMultiplier != 0 ||
						_durationColor->blueMultiplier != 0 
					)
					{
						_tweenColor = true;
					}
					else
					{
						_tweenColor = false;
					}
				}
				else if(_currentFrame->color)
				{
					_tweenColor = true;
					_durationColor->alphaOffset = -_currentFrame->color->alphaOffset;
					_durationColor->redOffset = -_currentFrame->color->redOffset;
					_durationColor->greenOffset = -_currentFrame->color->greenOffset;
					_durationColor->blueOffset = -_currentFrame->color->blueOffset;
					
					_durationColor->alphaMultiplier = 1 - _currentFrame->color->alphaMultiplier;
					_durationColor->redMultiplier = 1 - _currentFrame->color->redMultiplier;
					_durationColor->greenMultiplier = 1 - _currentFrame->color->greenMultiplier;
					_durationColor->blueMultiplier = 1 - _currentFrame->color->blueMultiplier;
				}
				else if(nextFrame->color)
				{
					_tweenColor = true;
					_durationColor->alphaOffset = nextFrame->color->alphaOffset;
					_durationColor->redOffset = nextFrame->color->redOffset;
					_durationColor->greenOffset = nextFrame->color->greenOffset;
					_durationColor->blueOffset = nextFrame->color->blueOffset;
					
					_durationColor->alphaMultiplier = nextFrame->color->alphaMultiplier - 1;
					_durationColor->redMultiplier = nextFrame->color->redMultiplier - 1;
					_durationColor->greenMultiplier = nextFrame->color->greenMultiplier - 1;
					_durationColor->blueMultiplier = nextFrame->color->blueMultiplier - 1;
				}
				else
				{
					_tweenColor = false;
				}
			}
				
			if(!_tweenTransform)
			{
				if(_animationState->blend)
				{
					transform->x = _originTransform->x + _currentFrame->transform->x;
					transform->y = _originTransform->y + _currentFrame->transform->y;
					transform->skewX = _originTransform->skewX + _currentFrame->transform->skewX;
					transform->skewY = _originTransform->skewY + _currentFrame->transform->skewY;
					transform->scaleX = _originTransform->scaleX + _currentFrame->transform->scaleX;
					transform->scaleY = _originTransform->scaleY + _currentFrame->transform->scaleY;
				
					pivot->x = _originPivot->x + _currentFrame->pivot->x;
					pivot->y = _originPivot->y + _currentFrame->pivot->y;
				}
				else
				{
					transform->x = _currentFrame->transform->x;
					transform->y = _currentFrame->transform->y;
					transform->skewX = _currentFrame->transform->skewX;
					transform->skewY = _currentFrame->transform->skewY;
					transform->scaleX = _currentFrame->transform->scaleX;
					transform->scaleY = _currentFrame->transform->scaleY;
					
					pivot->x = _currentFrame->pivot->x;
					pivot->y = _currentFrame->pivot->y;
				}
			}
				
			if(!_tweenColor)
			{
				if(_currentFrame->color)
				{
					_bone->updateColor(
						_currentFrame->color->alphaOffset, 
						_currentFrame->color->redOffset, 
						_currentFrame->color->greenOffset, 
						_currentFrame->color->blueOffset, 
						_currentFrame->color->alphaMultiplier, 
						_currentFrame->color->redMultiplier, 
						_currentFrame->color->greenMultiplier, 
						_currentFrame->color->blueMultiplier, 
						true
					);
				}
				else if(_bone->_isColorChanged)
				{
					_bone->updateColor(0, 0, 0, 0, 1, 1, 1, 1, false);
				}
			}
			_bone->arriveAtFrame(_currentFrame, this, _animationState, false);
		}
			
		if(_tweenTransform)
		{
			progress = (playedTime - _currentFramePosition) / _currentFrameDuration;
			if(_tweenEasing)
			{
				progress = getEaseValue(progress, _tweenEasing);
			}
			DBTransform* currentTransform = _currentFrame->transform;
			Vector2D* currentPivot = _currentFrame->pivot;
			if(_animationState->blend)
			{
				transform->x = _originTransform->x + currentTransform->x + _durationTransform->x * progress;
				transform->y = _originTransform->y + currentTransform->y + _durationTransform->y * progress;
				transform->skewX = _originTransform->skewX + currentTransform->skewX + _durationTransform->skewX * progress;
				transform->skewY = _originTransform->skewY + currentTransform->skewY + _durationTransform->skewY * progress;
				transform->scaleX = _originTransform->scaleX + currentTransform->scaleX + _durationTransform->scaleX * progress;
				transform->scaleY = _originTransform->scaleY + currentTransform->scaleY + _durationTransform->scaleY * progress;
				
				pivot->x = _originPivot->x + currentPivot->x + _durationPivot->x * progress;
				pivot->y = _originPivot->y + currentPivot->y + _durationPivot->y * progress;
			}
			else
			{
				transform->x = currentTransform->x + _durationTransform->x * progress;
				transform->y = currentTransform->y + _durationTransform->y * progress;
				transform->skewX = currentTransform->skewX + _durationTransform->skewX * progress;
				transform->skewY = currentTransform->skewY + _durationTransform->skewY * progress;
				transform->scaleX = currentTransform->scaleX + _durationTransform->scaleX * progress;
				transform->scaleY = currentTransform->scaleY + _durationTransform->scaleY * progress;
				
				pivot->x = currentPivot->x + _durationPivot->x * progress;
				pivot->y = currentPivot->y + _durationPivot->y * progress;
			}
		}
			
		if(_tweenColor)
		{
			if(_currentFrame->color)
			{
					_bone->updateColor(
					_currentFrame->color->alphaOffset + _durationColor->alphaOffset * progress,
					_currentFrame->color->redOffset + _durationColor->redOffset * progress,
					_currentFrame->color->greenOffset + _durationColor->greenOffset * progress,
					_currentFrame->color->blueOffset + _durationColor->blueOffset * progress,
					_currentFrame->color->alphaMultiplier + _durationColor->alphaMultiplier * progress,
					_currentFrame->color->redMultiplier + _durationColor->redMultiplier * progress,
					_currentFrame->color->greenMultiplier + _durationColor->greenMultiplier * progress,
					_currentFrame->color->blueMultiplier + _durationColor->blueMultiplier * progress,
					true
				);
			}
			else
			{
				_bone->updateColor(
					_durationColor->alphaOffset * progress,
					_durationColor->redOffset * progress,
					_durationColor->greenOffset * progress,
					_durationColor->blueOffset * progress,
					1 + _durationColor->alphaMultiplier * progress,
					1 + _durationColor->redMultiplier * progress,
					1 + _durationColor->greenMultiplier * progress,
					1 + _durationColor->blueMultiplier * progress,
					false
				);
			}
		}
		return 2;
	}

	void TimelineState::clear(){
		this->updatef = &TimelineState::updateNothing;
		_bone = 0;
		_animationState = 0;
		_timeline = 0;
		_currentFrame = 0;
		_originTransform = 0;
		_originPivot = 0;
	}

	void TimelineState::update(float progress) {
		(this->*updatef)(progress);
	}

}