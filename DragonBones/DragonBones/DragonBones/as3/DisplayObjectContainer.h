#ifndef DISPLAYOBJECTCONTAINER_H
#define DISPLAYOBJECTCONTAINER_H

#include "DisplayObject.h"

namespace DragonBones {
	class DisplayObjectContainer : public DisplayObject {
	private:
		std::vector<DisplayObject*> children;
	public:
		DisplayObjectContainer();
		~DisplayObjectContainer();

		void addChild(DisplayObject* child);
		void addChildAt(DisplayObject* child, int index);
		//void broadcastEvent(Event* event); To be implemented
		bool contains(DisplayObject* child);
		DisplayObject* getChildAt(int index);
		DisplayObject* getChildByName(std::string name);
		int getChildIndex(DisplayObject* child);
		void removeChild(DisplayObject* child, bool dispose=false);
		void removeChildAt(int index, bool dispose = false);
		void render();
		int getNumberOfChildren(){return children.size();}

		glm::mat4 rec;
	};
}

#endif