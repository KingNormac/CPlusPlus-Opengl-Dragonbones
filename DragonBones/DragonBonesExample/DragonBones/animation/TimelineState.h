#ifndef TIMELINESTATE_H
#define TIMELINESTATE_H

#include <math.h>
#include <string>
#include <vector>
class Vector2D;
namespace DragonBones {
	class Bone;
	class TransformTimeline;
	class ColorTransform;
	class TransformFrame;
	class AnimationState;
	class DBTransform;
	class TimelineState{
	private:
		static const double M_PI;
		static const double HALF_PI;
		static const double DOUBLE_PI;
		static std::vector<TimelineState*> _pool;
		
		AnimationState* _animationState;
		Bone*_bone;
		TransformTimeline* _timeline;
		TransformFrame* _currentFrame;
		float _currentFramePosition;
		float _currentFrameDuration;
		DBTransform* _durationTransform;
		Vector2D* _durationPivot;
		ColorTransform* _durationColor;
		DBTransform* _originTransform;
		Vector2D* _originPivot;
		
		float _tweenEasing;
		bool _tweenTransform;
		bool _tweenColor;

		float _totalTime;

		int updateNothing(float progress);
		int updateSingle(float progress);
		int updateList(float progress);
		void clear();

	public:
		typedef int (TimelineState::*function)(float);
		DBTransform* transform;
		Vector2D* pivot;
		function updatef;

		static float getEaseValue(float value, float easing);
		static TimelineState* borrowObject();
		static void returnObject(TimelineState* timeline);
		static void clearStatic();

		TimelineState();
		~TimelineState();
		void fadeIn(Bone* bone, AnimationState* animationState, TransformTimeline* timeline);
		void fadeOut();

		void update(float progress);

	};
}

#endif