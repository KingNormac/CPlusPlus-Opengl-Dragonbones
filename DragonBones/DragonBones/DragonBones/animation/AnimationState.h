#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include <vector>
#include <string>
#include <map>

namespace DragonBones {
	class TimelineState;
	class Frame;
	class AnimationData;
	class Armature;
	class AnimationState {
	private:
		static std::vector<AnimationState*> _pool;
		static void clearStatic();
		void updateTimelineStates();
		void addTimelineState(std::string timelineName);
		void removeTimelineState(std::string timelineName);
		void clear();

		//std::vector<TimelineState*> _timelineStates;
		
		
		Armature* _armature;
		Frame* _currentFrame;
		//Object* _mixingTransforms;
		std::map<std::string, int> _mixingTransforms;
		int _fadeState;
		float _fadeInTime;
		float _fadeOutTime;
		float _fadeOutBeginTime;
		float _fadeOutWeight;
		bool _fadeIn;
		bool _fadeOut;
		bool _pauseBeforeFadeInComplete;

		
		AnimationData* _clip;
		int _loopCount;
		int _loop;
		int _layer;
		bool _isPlaying;
		bool _isComplete;
		float _totalTime;
		float _currentTime;
		float _timeScale;
		bool _displayControl;
	public:
		std::string _name;
		bool enabled;
		bool tweenEnabled;
		bool blend;
		std::string group;
		float weight;
		float _fadeWeight;

		std::map<std::string, TimelineState*> _timelineStates;

		static void returnObject(AnimationState* animationState);
		TimelineState* getTimelineState(std::string value);
		static AnimationState* borrowObject();
		std::string getName();
		AnimationData* getClip();
		int getLoopCount();
		int getLoop();
		int getLayer();
		bool isPlaying();
		bool isComplete();
		float getFadeInTime();
		float getTotalTime();
		
		
		void setCurrentTime(float value);
		void setTimeScale(float value);

		bool getDisplayControl();
		void setDisplayControl(bool value);
		
		AnimationState();
		~AnimationState();

		void fadeIn(Armature* armature, AnimationData* clip, float fadeInTime, float timeScale, int loop, int layer, bool displayControl, bool pauseBeforeFadeInComplete);
		
		void fadeOut(float fadeOutTime, bool pause = false);
		
		void play();
		void stop();
		int getMixingTransform(std::string timelineName);
		void addMixingTransform(std::string timelineName, int type=2, bool recursive=true);
		void removeMixingTransform(std::string timelineName="", bool recursive=true);
		bool advanceTime(float passedTime);
	};
};



#endif