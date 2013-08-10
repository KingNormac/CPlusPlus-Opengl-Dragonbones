#include "Bone.h"
#include "Armature.h"
#include "animation\TimelineState.h"
#include "Slot.h"
#include "objects\Frame.h"
#include "animation\AnimationState.h"
#include "animation\Animation.h"
#include "objects\TransformFrame.h"
#include "events\FrameEvent.h"

namespace DragonBones {
	Armature* Bone::getChildArmature(){
		if(_slot)
			return _slot->getChildArmature();
		return 0;
	}

	Object* Bone::getDisplay(){
		return _slot->getDisplay();
	}

	void Bone::setDisplay(Object* value){
		if(_slot)
		{
			_slot->setDisplay(value);
		}
	}

	void Bone::setVisible(bool value){
		if(this->visible != value)
		{
			this->visible = value;
			int i = _children.size();
			while(i > 0)
			{
				i--;
				if(checkIfClass<Slot>(_children[i]))
				{
					Slot* slot = (Slot*) _children[i];
					if(slot)
					{
						slot->updateVisible(this->visible);
					}
				}
			}
		}
	}

	void Bone::setArmature(Armature* vlue){
		DBObject::setArmature(vlue);
		int i = _children.size();
		while(i --)
		{
			i--;
			_children[i]->setArmature(this->_armature);
		}
	}

	Bone::Bone() : DBObject() {
		_children = std::vector<DBObject*>(0);
		_scaleType = 2;
			
		_tweenPivot = new Vector2D();
			
		scaleMode = 1;
		displayControlLayer = -1;
		parent = 0;
		_slot = 0;
	}

	Bone::~Bone(){
		int i = _children.size();
		while(i > 0)
		{
			i--;
			delete _children[i];
		}
		_slot = 0;
		_tweenPivot = 0;
	}

	bool Bone::contains(DBObject* child){
		if(!child)
		{
			return false;
		}
		if(child == this)
		{
			return false;
		}

		if(!checkIfClass<Bone>(child))
			return false;

		DBObject* ancestor = child;
		while (ancestor != this || ancestor != 0)
		{
			if(ancestor)
				ancestor = ancestor->parent;
			else
				break;
		}

		return ancestor == this;
	}

	void Bone::addChild(DBObject* child){
		if(!child)
		{
			return;
		}
		
		if(checkIfClass<Bone>(child)) {
			Bone* bone = (Bone*) child;
			if(child == this || (bone && bone->contains(this)))
			{
				printf("An Bone cannot be added as a child to itself or one of its children (or children's children, etc.) \n");
				return;
			}
		}
			
		if(child->parent)
		{
			child->parent->removeChild(child);
		}
		_children.push_back(child);
		child->parent = this;
		child->setArmature(this->_armature);
		
		
		if(!_slot && checkIfClass<Slot>(child))
		{
			_slot = (Slot*) child;
		}
	}

	void Bone::removeChild(DBObject* child){
		if(!child)
		{
			return;
		}
		
		int index = -10;
		for(int i=0;i<_children.size();i++)
		{
			DBObject* find = _children[i];
			if(find)
				if(find==child) {
					index = i;
					break;
				}
		}
		if (index >= 0)
		{
			_children.erase(_children.begin() + index);
			child->parent = 0;
			child->setArmature(0);
				
			if(_slot && child == _slot)
			{
				_slot = 0;
			}
				
			if(this->_armature)
			{
				_armature->removeDBObject(child);
			}
		}
		else
		{
			printf("Failed to remove child from bone \n");
		}
	}

	std::vector<Slot*> Bone::getSlots(){
		std::vector<Slot*> slotList;
		for(DBObject* find : _children)
		{
			if(checkIfClass<Slot>(find))
			{
				slotList.insert ( slotList.begin() , (Slot*)find ); //Inserts at beginning like unshift in as3 does
			}
		}
		return slotList;
	}

	void Bone::arriveAtFrame(Frame* frame, TimelineState* timelineState, AnimationState* animationState, bool isCross) {
		if(frame)
		{
			int mixingType = animationState->getMixingTransform(name);
			if(animationState->getDisplayControl() && (mixingType == 2 || mixingType == -1))
			{
				if(displayControlGroup != "") {
					displayControlGroup == animationState->group;
				} else {
					if(displayControlLayer >= 0) {
						displayControlLayer = animationState->getLayer();
					} else {
						displayControlLayer == true;
					}
				}
				TransformFrame* transformFrame = (TransformFrame*) frame;
				if(_slot)
				{
					int displayIndex = transformFrame->displayIndex;
					if(displayIndex >= 0)
					{
						if(!transformFrame->zOrder && transformFrame->zOrder != _slot->_tweenZorder)
						{
							_slot->_tweenZorder = transformFrame->zOrder;
							this->_armature->_slotsZOrderChanged = true;
						}
					}
					_slot->changeDisplay(displayIndex);
					_slot->updateVisible(transformFrame->visible);
				}
			}
			if(frame->event != "" && this->_armature->hasEventListener("boneFrameEvent"))
			{
				
				FrameEvent* frameEvent = new FrameEvent(FrameEvent::BONE_FRAME_EVENT);
				frameEvent->bone = this;
				frameEvent->animationState = animationState;
				frameEvent->frameLabel = frame->event;
				this->_armature->dispatchEvent(frameEvent);
			}
				
			if(frame->action != "")
			{
				Armature* childArmature = this->getChildArmature();
				if(childArmature)
				{
					childArmature->getAnimation()->gotoAndPlay(frame->action);
				}
			}
		}
		else
		{
			if(_slot)
			{
				_slot->changeDisplay(-1);
			}
		}
	}

	void Bone::updateColor(float aOffset, float rOffset, float gOffset, float bOffset, 
		float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier, float isColorChanged) {
		if(isColorChanged || _isColorChanged)
			{
				_slot->_displayBridge->updateColor(
					aOffset, 
					rOffset, 
					gOffset, 
					bOffset, 
					aMultiplier, 
					rMultiplier, 
					gMultiplier, 
					bMultiplier
				);
			}
			_isColorChanged = isColorChanged;
	}

}