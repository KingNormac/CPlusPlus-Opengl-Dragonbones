#include "DBObject.h"
#include "../Armature.h"
#include "../Bone.h"
#include "../utils/TransformUtil.h"

namespace DragonBones {

	bool DBObject::isVisible(){
		return visible;
	}

	void DBObject::setVisible(bool value){
		visible = value;
	}

	Armature* DBObject::getArmature(){
		return _armature;
	}

	void DBObject::setArmature(Armature* armature){
		if(_armature)
		{
			_armature->removeDBObject(this);
		}
		_armature = armature;
		if(_armature)
		{
			_armature->addDBObject(this);
		}
	}

	DBObject::DBObject() : EventDispatcher() {
		global = new DBTransform();
		origin = new DBTransform();
		offset = new DBTransform();
		_tween = new DBTransform();
		_tween->scaleX = _tween->scaleY = 0;
			
		_globalTransformMatrix = TranformUtils::transformToMatrix(global);
		fixedRotation = false;
		setVisible(true);
		_armature = 0;
		parent = 0;
	}

	DBObject::~DBObject(){
		//userData = null;
		parent = 0;
		_armature = 0;
		global = 0;
		origin = 0;
		offset = 0;
		_tween = 0;
		_globalTransformMatrix = glm::mat4(1.0f);
	}

	void DBObject::update(){
		global->scaleX = (origin->scaleX + _tween->scaleX) * offset->scaleX;
		global->scaleY = (origin->scaleY + _tween->scaleY) * offset->scaleY;
			
		if(parent)
		{
			float x = origin->x + offset->x + _tween->x;
			float y = origin->y + offset->y + _tween->y;
			glm::mat4 parentMatrix = parent->_globalTransformMatrix;

			_globalTransformMatrix[3][0] = global->x = parentMatrix[0][0] * x + parentMatrix[1][0] * y + parentMatrix[3][0];
			_globalTransformMatrix[3][1] = global->y = parentMatrix[1][1] * y + parentMatrix[0][1] * x + parentMatrix[3][1];
			
				
			if(fixedRotation)
			{
				//printf("K \n");
				global->skewX = origin->skewX + offset->skewX + _tween->skewX;
				global->skewY = origin->skewY + offset->skewY + _tween->skewY;
			}
			else
			{
				//printf("KK \n");
				global->skewX = origin->skewX + offset->skewX + _tween->skewX + parent->global->skewX;
				global->skewY = origin->skewY + offset->skewY + _tween->skewY + parent->global->skewY;
			}
				
			if(parent->scaleMode >= _scaleType)
			{
				global->scaleX *= parent->global->scaleX;
				global->scaleY *= parent->global->scaleY;
			}
		}
		else
		{
			_globalTransformMatrix[3][0] = global->x = origin->x + offset->x + _tween->x;
			_globalTransformMatrix[3][1] = global->y = origin->y + offset->y + _tween->y;
				
			global->skewX = origin->skewX + offset->skewX + _tween->skewX;
			global->skewY = origin->skewY + offset->skewY + _tween->skewY;
		}
			
		_globalTransformMatrix[0][0] =  global->scaleX * cos(global->skewY); //a
		_globalTransformMatrix[0][1] =  global->scaleX * sin(global->skewY); //b
		_globalTransformMatrix[1][0] = -global->scaleY * sin(global->skewX); //c
		_globalTransformMatrix[1][1] =  global->scaleY * cos(global->skewX); //d
		//TranformUtils::printMatrix(_globalTransformMatrix);
		//_globalTransformMatrix = TranformUtils::transformToMatrix(global);
	}
}