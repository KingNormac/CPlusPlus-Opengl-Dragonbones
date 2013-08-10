#ifndef SLOT_H
#define SLOT_H

#include <vector>
#include <string>
#include "objects\DisplayData.h"
#include "display\DisplayBridge.h"
#include "core\DBObject.h"
#include "as3\Object.h"


namespace DragonBones {
	class Slot : public DBObject{
	private:
		
		
		
		
		

		bool _isHideDisplay;
		
		int _displayIndex;
		 //Image or Armature

		

		//void setArmature(Armature* value);
	public:
		float _originZOrder;
		std::vector<Object*> displayList;
		std::vector<DisplayData*> _displayDataList;
		float _offsetZOrder;

		float getZOrder();
		void setZOrder(float value);
		float _tweenZorder;

		DisplayBridge* _displayBridge;

		Object* getDisplay();
		void setDisplay(Object* value);

		Armature* getChildArmature();
		void setChildArmature(Armature* value);

		std::vector<Object*> getDisplayList();
		void setDisplayList(std::vector<Object*> value);

		void setVisible(bool value);
		void setArmature(Armature* value);

		bool _isDisplayOnStage;

		Slot(DisplayBridge* displayBridge);
		~Slot();

		void update();
		void updateVisible(bool value);

		void changeDisplayList(std::vector<Object*> displayList);

		void changeDisplay(int displayIndex);
	};
}

#endif