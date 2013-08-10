#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <string>
#include "../glm/glm.hpp"
#include "../objects/DBTransform.h"
#include "../as3/EventDispatcher.h"
#include "../as3/Object.h"

namespace DragonBones {

	class Armature;
	class Bone;

	class DBObject : public EventDispatcher {
	private:
		
	protected:
		int _scaleType;
		bool visible;
		Armature* _armature;
		
	public:
		glm::mat4 _globalTransformMatrix;
		std::string name;
		//Object* userData; dont know how this is used yet
		bool fixedRotation;
		bool _isColorChanged;
		DBTransform* _tween;
		DBTransform* global;
		DBTransform* origin;
		DBTransform* offset; //is offset and node?

		bool isVisible();
		virtual void setVisible(bool value);
		
		Bone* parent;

		Armature* getArmature();
		virtual void setArmature(Armature* armature);

		DBObject();
		~DBObject();

		void update();

	};
}


#endif