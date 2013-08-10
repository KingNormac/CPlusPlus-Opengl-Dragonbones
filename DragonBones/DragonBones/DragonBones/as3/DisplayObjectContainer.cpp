#include "DisplayObjectContainer.h"
#include "../utils/TransformUtil.h"

namespace DragonBones {
	DisplayObjectContainer::DisplayObjectContainer() : DisplayObject(){
	}

	DisplayObjectContainer::~DisplayObjectContainer(){
	}

	void DisplayObjectContainer::addChild(DisplayObject* child){
		if(child) {
			children.push_back(child);
			child->parent = this;
		}
	}

	void DisplayObjectContainer::addChildAt(DisplayObject* child, int index){
		if(child) {
			children[index] = child;
			child->parent = this;
		}
	}

	bool DisplayObjectContainer::contains(DisplayObject* child){
		for(DisplayObject* found : children)
		{
			if(child==found)
				return true;
		}
		return false;
	}

	DisplayObject* DisplayObjectContainer::getChildAt(int index){
		DisplayObject* found = children[index];
		if(found)
			return found;
		return 0;
	}

	DisplayObject* DisplayObjectContainer::getChildByName(std::string name){
		for(DisplayObject* found : children)
		{
			if(found)
				if(found->name==name)
					return found;
		}
		return 0;
	}

	int DisplayObjectContainer::getChildIndex(DisplayObject* child){
		for(int i=0;i<children.size();i++)
		{
			if(children[i]==child)
				return i;
		}
		return -1;
	}

	void DisplayObjectContainer::removeChild(DisplayObject* child, bool dispose){
		for(int i=0;i<children.size();i++)
		{
			DisplayObject* found = children[i];
			if(found) {
				if(found==child)
				{
					found->parent = 0;
					children.erase(children.begin()+i);
					if(dispose)
					{
						delete child;
					}
				}
			}
		}
	}

	void DisplayObjectContainer::removeChildAt(int index, bool dispose) {
		DisplayObject* found = children[index];
		children.erase(children.begin()+index);
		if(found)
		{
			found->parent = 0;
			if(dispose)
			{
				delete found;
			}
		}
	}

	void DisplayObjectContainer::render() {
		if(visible) {
			if(parent) {
				rec = this->parent->rec * this->getTransformationMatrix();
			} else {
				rec = this->getTransformationMatrix();
			}

			for(DisplayObject* child : children)
			{
				if(child)
				{
					if(child->visible)
					{
						child->render();
					}
				}
			}
		}
	}
}