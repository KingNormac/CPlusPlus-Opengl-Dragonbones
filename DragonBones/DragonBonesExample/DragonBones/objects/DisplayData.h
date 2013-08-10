#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <string>
#include "DBTransform.h"
#include "../as3/Vector2D.h"

namespace DragonBones {

//	const std::string ARMATURE = "armature";
//	const std::string IMAGE = "image";

	class DisplayData {
	public:
		std::string name;
		std::string type;
		DBTransform* transform;
		Vector2D* pivot;

		DisplayData() {
			name = "";
			type = "";
			transform = new DBTransform();
			pivot = new Vector2D(0,0);
		}

		~DisplayData() {
			delete[] transform;
		}
	};

};

#endif