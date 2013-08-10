#include "Slot.h"
#include "Armature.h"
#include "Bone.h"
#include "animation\Animation.h"
#include "as3\DisplayObjectContainer.h"

namespace DragonBones {

	float Slot::getZOrder() {
		return _originZOrder + _tweenZorder + _offsetZOrder;
	}

	void Slot::setZOrder(float value) {
		if(getZOrder() != value) {
			_offsetZOrder = value - _originZOrder - _tweenZorder;
			if(this->_armature)
			{
				this->_armature->_slotsZOrderChanged = true;
			}
		}
	}

	Object* Slot::getDisplay() {
		Object* display = displayList[_displayIndex];
		if(checkIfClass<Armature>(display)) 
		{
			Armature* d = (Armature*) display;
			return (Object*)d->getDisplay();
		}
		return display;
	}

	void Slot::setDisplay(Object* value) {
		displayList[_displayIndex] = value;
			
		if(_displayBridge->getDisplay())
		{
			_displayBridge->setDisplay(value);
		}
		else
		{
			_displayBridge->setDisplay(value);
			if(this->_armature)
			{
				_displayBridge->addDisplay(this->_armature->getDisplay());
				this->_armature->_slotsZOrderChanged = true;
			}
		}
		if(!_isHideDisplay && _displayBridge->getDisplay())
		{
			_isDisplayOnStage = true;
		}
		else
		{
			_isDisplayOnStage = false;
		}
	}

	Armature* Slot::getChildArmature() {
		if(displayList[_displayIndex])
			if(checkIfClass<Armature>(displayList[_displayIndex]))
				return (Armature*)displayList[_displayIndex];
		return 0;
	}

	void Slot::setChildArmature(Armature* value) {
		displayList[_displayIndex] = value;
		if(value)
		{
			_displayBridge->setDisplay((Object*)value->getDisplay());
		}
	}

	std::vector<Object*> Slot::getDisplayList() {
		return displayList;
	}

	void Slot::setDisplayList(std::vector<Object*> value) {
		displayList.resize(value.size());
		int i = value.size();
		while(i --)
		{
			displayList[i] = value[i];
		}
			
		if(_displayIndex >= 0)
		{
			_displayIndex = -1;
			changeDisplay(_displayIndex);
		}
	}

	void Slot::setVisible(bool value) {
		if(value != this->visible)
		{
			this->visible = value;
			updateVisible(this->visible);
		}
	}

	void Slot::setArmature(Armature* value) {
		DBObject::setArmature(value);
		if(this->_armature)
		{
			this->_armature->_slotsZOrderChanged = true;
			_displayBridge->addDisplay(this->_armature->getDisplay());
		}
		else
		{
			_displayBridge->removeDisplay();
		}
	}

	Slot::Slot(DisplayBridge* displayBridge) : DBObject() {
		parentClass = "Slot";
		_displayBridge = displayBridge;
		displayList.clear();
		_displayIndex = -1;
		_scaleType = 1;
			
		_originZOrder = 0;
		_tweenZorder = 0;
		_offsetZOrder = 0;
			
		_isDisplayOnStage = false;
		_isHideDisplay = false;
	}

	Slot::~Slot() {
		DBObject::~DBObject();
		delete _displayBridge;
		displayList.clear();
		_displayBridge = 0;
		_displayDataList.clear();
	}

	void Slot::update() {
		DBObject::update();
			
		if(_isDisplayOnStage)
		{
			Armature* child = this->getChildArmature();
			if(parent->_tweenPivot)
			{
				float pivotX = parent->_tweenPivot->x;
				float pivotY = parent->_tweenPivot->y;
				if(parent)
				{
					glm::mat4 parentMatrix = parent->_globalTransformMatrix;
					_globalTransformMatrix[3][0] += parentMatrix[0][0] * pivotX + parentMatrix[1][0] * pivotY;
					_globalTransformMatrix[3][1] += parentMatrix[0][1] * pivotX + parentMatrix[1][1] * pivotY;
				}
			}
				
			_displayBridge->updateTransform(this->_globalTransformMatrix, this->global);
			if(child)
			{
				child->getDisplay()->setTransformationMatrix(this->_globalTransformMatrix);
			}
		}
	}

	void Slot::updateVisible(bool value) {
		_displayBridge->setVisible(this->parent->isVisible() && this->visible && value);
	}

	void Slot::changeDisplayList(std::vector<Object*> displayList) {
		this->displayList = displayList;
	}

	void Slot::changeDisplay(int displayIndex) {
		if(displayIndex < 0)
		{
			if(!_isHideDisplay)
			{
				_isHideDisplay = true;
				_displayBridge->removeDisplay();
			}
		}
		else
		{
			if(_isHideDisplay)
			{
				_isHideDisplay = false;
				if(this->_armature)
				{
					int in = this->_armature->getSlotIndexOf(this);
					if(in > -1) {
						_displayBridge->addDisplay(this->_armature->getDisplay(), in);
						this->_armature->_slotsZOrderChanged = true;
					}
				}
			}
				
			int length = displayList.size();
			if(displayIndex >= length && length > 0)
			{
				displayIndex = length - 1;
			}
			if(_displayIndex != displayIndex)
			{
				_displayIndex = displayIndex;
				Object* content = displayList[_displayIndex];
				if(checkIfClass<Armature>(content))
				{
					Armature* contentArmature = (Armature*) content;
					_displayBridge->setDisplay((Object*)(contentArmature->getDisplay()));
				}
				else
				{
					_displayBridge->setDisplay(content);
				}
				
				if(_displayDataList.size() > 0 && _displayIndex <= _displayDataList.size())
				{
					this->origin->copy(_displayDataList[_displayIndex]->transform);
				}
			}
		}
			
		if(!_isHideDisplay && _displayBridge->getDisplay())
		{
			if(!_isDisplayOnStage)
			{
				_isDisplayOnStage = true;
				if(getChildArmature())
				{
					getChildArmature()->getAnimation()->play();
				}
			}
		}
		else if(_isDisplayOnStage)
		{
			_isDisplayOnStage = false;
			if(getChildArmature())
			{
				getChildArmature()->getAnimation()->stop();
				getChildArmature()->getAnimation()->_lastAnimationState = 0;
			}
		}
	}
}