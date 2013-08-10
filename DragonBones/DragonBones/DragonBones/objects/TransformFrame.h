#ifndef TRANSFORMFRAME_H
#define TRANSFORMFRAME_H

#include "DBTransform.h"
#include "../as3/ColorTransform.h"
#include "../as3/Vector2D.h"
#include "Frame.h"

namespace DragonBones {
	class TransformFrame : public Frame {
	public:
		float tweenEasing;
		float tweenRotate;
		int displayIndex;
		bool visible;
		int zOrder;

		DBTransform* global;
		DBTransform* transform;
		Vector2D* pivot;
		ColorTransform* color;


		TransformFrame() : Frame() {
			tweenEasing = _Nan._Float;
			tweenRotate = 0;
			displayIndex =0;
			visible = true;
			zOrder = 0;

			position = 0;
			duration = 0;

			global = new DBTransform();
			transform = new DBTransform();
			pivot = new Vector2D(0,0);
			color = 0;
		}

		~TransformFrame() {
			delete[] global;
			delete[] transform;
			delete[] color;
		}

	};
};

#endif