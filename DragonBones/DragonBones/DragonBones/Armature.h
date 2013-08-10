#ifndef ARMATURE_H
#define ARMATURE_H

#include "as3\Object.h"
#include "animation\IAnimatable.h"
#include "as3\EventDispatcher.h"

namespace DragonBones {
	class Animation;
	class Frame;
	class AnimationState;
	class TimelineState;
	class Slot;
	class Bone;
	class DBObject;
	class DisplayObjectContainer;
	class Armature : public EventDispatcher, public IAnimatable {
	private:
		
		
		DisplayObjectContainer* _display;
		Animation* _animation;

		
		void sortBoneList();

		
		int sortSlot(Slot* sort1, Slot* sort2);

	public:
		std::vector<Slot*> _slotList;
		std::vector<Bone*> _boneList;
		
		bool _slotsZOrderChanged;
		std::string name;
		//Object* userData;

		DisplayObjectContainer* getDisplay();
		Animation* getAnimation();

		Armature(Object* display);
		~Armature();

		void advanceTime(float passedTime);

		std::vector<Slot*> getSlots(bool returnCopy=true);
		std::vector<Bone*> getBones(bool returnCopy=true);

		Slot* getSlot(std::string slotName);
		Slot* getSlotByDisplay(Object* display);
		void removeSlot(Slot* slot);
		void removeSlotByName(std::string slotName);

		Bone* getBone(std::string boneName);
		Bone* getBoneByDisplay(Object* display);
		void removeBone(Bone* bone);
		void removeBoneByName(std::string boneName);

		void addChild(DBObject* object, std::string parentName="");
		void addBone(Bone* bone, std::string parentName="");

		void updateSlotsZOrder();
		void addDBObject(DBObject* object);
		void removeDBObject(DBObject* object);

		int getSlotIndexOf(Slot* slot);
		void arriveAtFrame(Frame* frame, TimelineState* timelineState, AnimationState* animationState, bool isCross);
	};
}
#endif