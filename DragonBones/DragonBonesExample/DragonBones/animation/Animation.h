#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include "AnimationState.h"
#include "../Armature.h"

namespace DragonBones {
	class Animation {
	private:
		std::vector<std::vector<AnimationState*>> _animationLayer;
		Armature* _armature;
		bool _isPlaying;
		
		std::vector<std::string> _animationList;
		std::vector<AnimationData*> _animationDataList;
		float _timeScale;

	
		int addLayer(int layer);
		void addState(AnimationState* animationState);
		void removeState(AnimationState* animationState);

	public:
		AnimationState* _lastAnimationState;
		static const std::string FADE_OUT_NONE;
		static const std::string FADE_OUT_SAME_LAYER;
		static const std::string FADE_OUT_ALL_LAYER;

		bool tweenEnabled;

		std::vector<std::string> getMovementList();
		std::string getMovementID();

		void setStatesDisplayControl(AnimationState* animationState);

		AnimationState* getLastAnimationState();
		std::vector<std::string> getAnimationList();

		bool isPlaying();
		bool isComplete();

		std::vector<AnimationData*> getAnimationDataList();
		void setAnimationDataList(std::vector<AnimationData*> value);

		float getTimeScale();
		void setTimeScale(float value);

		Animation(Armature* armature);
		~Animation();

		AnimationState* gotoAndPlay(
			std::string animationName, 
			float fadeInTime = -1, 
			float duration = -1, 
			float loop = _Nan._Float, 
			int layer = 0, 
			std::string playMode = FADE_OUT_SAME_LAYER,
			bool displayControl = true,
			bool pauseFadeOut = true,
			bool pauseFadeIn = true
		);

		void play();
		void stop();

		AnimationState* getState(std::string name, int layer=0);
		void advanceTime(float passedTime);
	};
}

#endif