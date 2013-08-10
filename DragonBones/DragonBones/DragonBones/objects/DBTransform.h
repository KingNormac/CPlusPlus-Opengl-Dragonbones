#ifndef DBTRANSFORM_H
#define DBTRANSFORM_H

#include <sstream>
#include <string>

namespace DragonBones {

	class DBTransform {
	public:
		float x;
		float y;
		float skewX;
		float skewY;
		float scaleX;
		float scaleY;
		
		float getRotation(){return skewX;}
		void setRotation(float value){skewX=skewY=value;}

		float getScaleX(){return scaleX;}
		float setScaleX(float value){scaleX=value;}

		DBTransform() {
			x=0;
			y=0;
			skewX=0;
			skewY=0;
			scaleX=1;
			scaleY=1;
		}

		void copy(DBTransform* transform) {
			x = transform->x;
			y = transform->y;
			skewX= transform->skewX;
			skewY = transform->skewY;
			scaleX = transform->scaleX;
			scaleY = transform->scaleY;
		}

		std::string toString() {
			std::stringstream ss;
			ss << "x:" << x << " y:" << y << " skewX:" << skewX << " skewY:" << skewY << " scaleX:" << scaleX << " scaleY:" << scaleY << " \n";
			return ss.str();
		}
	};

};
#endif