#include "Armature.h"
#include "objects\Frame.h"
#include "animation\AnimationState.h"
#include "animation\TimelineState.h"
#include "animation\Animation.h"
#include "Slot.h"
#include "Bone.h"
#include "as3\DisplayObjectContainer.h"
#include <algorithm> 
#include "events\FrameEvent.h"
#include "events\ArmatureEvent.h"

namespace DragonBones  {
	DisplayObjectContainer* Armature::getDisplay(){
		return _display;
	}

	Animation* Armature::getAnimation(){
		return _animation;
	}

	Armature::Armature(Object* display) : EventDispatcher(), IAnimatable(){
		_display = (DisplayObjectContainer*)display;
			
		_animation = new Animation(this);
		_slotsZOrderChanged = false;
			
		_slotList = std::vector<Slot*>();
		_boneList = std::vector<Bone*>();
	}

	Armature::~Armature(){
		//userData = null;
		
		delete _animation;
		_slotList.clear();
		_boneList.clear();
			
		_animation = 0;
	}

	void Armature::advanceTime(float passedTime){
		_animation->advanceTime(passedTime);
		

		for(Bone* bone : _boneList)
		{
			bone->update();
		}

			
		
		for(Slot* slot : _slotList)
		{
			slot->update();
			if(slot->_isDisplayOnStage)
			{
				Armature* childArmature = slot->getChildArmature();
				if(childArmature)
				{
					childArmature->advanceTime(passedTime);
				}
			}
		}

			
		if(_slotsZOrderChanged)
		{
			updateSlotsZOrder();
				
			if(this->hasEventListener(ArmatureEvent::Z_ORDER_UPDATED))
			{
				this->dispatchEvent(new ArmatureEvent(ArmatureEvent::Z_ORDER_UPDATED));
			}
		}

	}

	std::vector<Slot*> Armature::getSlots(bool returnCopy){
		return _slotList;
	}

	std::vector<Bone*> Armature::getBones(bool returnCopy){
		return _boneList;
	}

	Slot* Armature::getSlot(std::string slotName){
		for(Slot* slot : _slotList)
		{
			if(slot->name == slotName)
			{
				return slot;
			}
		}
		return 0;
	}

	Slot* Armature::getSlotByDisplay(Object* display){
		for(Slot* found : _slotList)
		{
			if(found == display)
			{
				return found;
			}
		}
		return 0;
	}

	void Armature::removeSlot(Slot* slot){
		for(Slot* found : _slotList)
		{
			if(found == slot) {
				slot->parent->removeChild(slot);
				return;
			}
		}
	}

	void Armature::removeSlotByName(std::string slotName){
		if((slotName == "") || (slotName == " "))
		{
			return;
		}
			
		Slot* slot = getSlot(slotName);
		if(slot)
		{
			removeSlot(slot);
		}
	}

	Bone* Armature::getBone(std::string boneName){
		for(Bone* bone : _boneList)
		{
			if(bone->name == boneName)
			{
				return bone;
			}
		}
		return 0;
	}

	Bone* Armature::getBoneByDisplay(Object* display){
		Slot* slot = getSlotByDisplay(display);
		if(slot)
			return slot->parent;
		return 0;
	}

	void Armature::removeBone(Bone* bone){
		for(Bone* found : _boneList)
		{
			if(found == bone) 
			{
				bone->parent->removeChild(bone);
				return;
			}
		}
	}

	void Armature::removeBoneByName(std::string boneName){
		if((boneName == "") || (boneName == " "))
		{
			return;
		}
			
		Bone* bone = getBone(boneName);
		if(bone)
		{
			removeBone(bone);
		}
	}

	void Armature::addChild(DBObject* object, std::string parentName){
		if(!object)
		{
			return;
		}
			
		if(parentName != "")
		{
			Bone* boneParent = getBone(parentName);
			if (boneParent)
			{
				boneParent->addChild(object);
			}
		}
		else
		{
			if(object->parent)
			{
					object->parent->removeChild(object);
			}
			object->setArmature(this);
		}
	}

	void Armature::addBone(Bone* bone, std::string parentName){
		addChild(bone, parentName);
	}

	void Armature::updateSlotsZOrder(){
		std::vector<Slot*> sortList(_slotList.size());
		for(Slot* slot : _slotList)
		{
			sortList[(int)slot->getZOrder()] = slot;
			if(slot->_isDisplayOnStage)
			{
				slot->_displayBridge->addDisplay(_display);
			}
		}
			
		_slotsZOrderChanged = false;
	}

	void Armature::addDBObject(DBObject* object){
		if(checkIfClass<Slot>(object))
		{
			Slot* slot = (Slot*)object;
			if(!getSlot(slot->name))
			{
				_slotList.push_back(slot);
			}
		}
		else if(checkIfClass<Bone>(object))
		{
			Bone* bone = (Bone*) object;
			if(!getBone(bone->name))
			{
				_boneList.push_back(bone);
				sortBoneList();
			}
		}
	}

	void Armature::removeDBObject(DBObject* object){
		if(checkIfClass<Slot>(object))
		{
			Slot* slot = (Slot*) object;
			int index = -1;
			for(int i=0;i<_slotList.size();i++)
			{
				Slot* found = _slotList[i];
				if(found == slot) {
					index = i;
					break;
				}
			}
			if(index >= 0)
			{
				_slotList.erase(_slotList.begin() + index);
			}
		}
		else if(checkIfClass<Bone>(object))
		{
			Bone* bone = (Bone*) object;
			int index = -1;
			for(int i=0;i<_boneList.size();i++)
			{
				Bone* found = _boneList[i];
				if(found == bone) {
					index = i;
					break;
				}
			}
			if(index >= 0)
			{
				_boneList.erase(_boneList.begin() + index);
			}
		}
	}

	void Armature::sortBoneList(){
		std::vector<std::pair<int, Bone*>> sortMap; // maps will auto sort by the ints
		if(_boneList.size() == 0) {
			return;
		}
		
		for(Bone* bone : _boneList) {
			int level = 0;
			Bone* boneParent = bone;
			while(boneParent) {
				level++;
				boneParent = boneParent->parent;
			}
			sortMap.push_back(std::make_pair(level, bone));
		}

		std::sort(sortMap.begin(), sortMap.end());

		for(int s=0; s<sortMap.size(); s++) {
			_boneList[s] = sortMap[s].second;
		}		
	}

	void Armature::arriveAtFrame(Frame* frame, TimelineState* timelineState, AnimationState* animationState, bool isCross){
		
		if(frame->event != "" && this->hasEventListener(FrameEvent::ANIMATION_FRAME_EVENT))
		{
			FrameEvent* frameEvent = new FrameEvent(FrameEvent::ANIMATION_FRAME_EVENT);
			frameEvent->animationState = animationState;
			frameEvent->frameLabel = frame->event;
			dispatchEvent(frameEvent);
		}
			
		/*if(frame.sound && this.hasEventListener(SoundEvent.SOUND))
		{
			var soundEvent:SoundEvent = new SoundEvent(SoundEvent.SOUND);
			soundEvent.armature = this;
			soundEvent.animationState = animationState;
			soundEvent.sound = frame.sound;
			_soundManager.dispatchEvent(soundEvent);
		}*/
			
		if(frame->action != "" && animationState->isPlaying())
		{
			getAnimation()->gotoAndPlay(frame->action);
		}
	}

	int Armature::sortSlot(Slot* sort1, Slot* sort2){
		return sort1->getZOrder() < sort2->getZOrder()?1: -1;
	}

	int Armature::getSlotIndexOf(Slot* slot) {
		for(int i=0;i<_slotList.size();i++) {
			Slot* found = _slotList[i];
			if(found == slot) {
				return i;
			}
		}
		return -10;
	}
}