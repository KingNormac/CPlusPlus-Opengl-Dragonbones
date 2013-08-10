#ifndef BONE_H
#define BONE_H

#include <string>
#include "as3\Object.h"
#include "core\DBObject.h"
#include "as3/Vector2D.h"


namespace DragonBones {
	class TimelineState;
	class Slot;
	class Frame;
	class AnimationState;
	class TimelineState;
	class Bone : public DBObject {
	private:
		std::vector<DBObject*> _children;
		Slot* _slot;
		
		
	public:
		Vector2D* _tweenPivot;
		int scaleMode; //0,1,2
		int displayControlLayer;
		std::string displayControlGroup;

		Armature* getChildArmature();

		Object* getDisplay();
		void setDisplay(Object* value);

		void setVisible(bool value);

		void setArmature(Armature* vlue);

		Bone();
		~Bone();

		bool contains(DBObject* child);
		void addChild(DBObject* child);
		void removeChild(DBObject* child);

		std::vector<Slot*> getSlots();
		
		void arriveAtFrame(Frame* frame, TimelineState* timelineState, AnimationState* animationState, bool isCross);
		void updateColor(float aOffset, float rOffset, float gOffset, float bOffset, 
			float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier, float isColorChanged);
	};
}


#endif