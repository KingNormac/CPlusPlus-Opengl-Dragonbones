#ifndef BONEDATA_H
#define BONEDATA_H

#include <string>
#include "DBTransform.h"
#include "../as3/Vector2D.h"

namespace DragonBones {
	class BoneData {
	public:
		std::string name;
		std::string parent;
		float length;

		DBTransform* global;
		DBTransform* transform;
		Vector2D* pivot;

		BoneData() {
			length = 0;
			global = new DBTransform();
			transform = new DBTransform();
			pivot = new Vector2D(0,0);
			parent = "";
		}

		~BoneData() {
			delete[] global;
			delete[] transform;
		}
	};
};


#endif