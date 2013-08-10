#ifndef DISPLAYOBJECT_H
#define DISPLAYOBJECT_H

#include <string>
#include "../as3/Object.h"
#include "../as3/EventDispatcher.h"
#include "../glm/glm.hpp"

namespace DragonBones {
	class DisplayObjectContainer;
	class DisplayObject : public EventDispatcher {
	private:
		float alpha;
		DisplayObject* base;
 	 	//float height;
		
 	 	float pivotX;
		float pivotY;
		DisplayObject* root;
 	 	float rotation;
 	 	float scaleX;
		float scaleY;
		float skewX;
		float skewY;
 	 	glm::mat4 transformationMatrix;
 	 	
 	 	//float width;
 	 	float x;
 	 	float y;
	public:
		DisplayObjectContainer* parent;
		DisplayObject();
		~DisplayObject();

		void setX(float value);
		void setY(float value);
		float getX();
		float getY();

		std::string name;

		glm::mat4 getTransformationMatrix();
		void setTransformationMatrix(glm::mat4 value);

		void setPivotX(float value);
		void setPivotY(float value);
		float getPivotX();
		float getPivotY();

		void setScaleX(float value);
		float getScaleX();

		void setRotation(float value);

		bool visible;
		void removeFromParent();
		virtual void render()=0;
	};
}

#endif
//std::string blendMode; //Have not implemented
//Rectangle bounds; //Have not implemented
//bool hasVisibleArea; //Have not implemented
//Stage stage; //Have not implemented
//bool touchable; Have not implemented