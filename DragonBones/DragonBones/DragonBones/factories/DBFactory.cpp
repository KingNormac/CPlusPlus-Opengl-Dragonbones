#include "DBFactory.h"
#include "../textures/TextureAtlas.h"
#include "../Armature.h"
#include "../Slot.h"
#include "../objects/SkeletonData.h"
#include "../as3/DisplayObjectContainer.h"
#include "../as3/Image.h"
#include "../Bone.h"
#include "../animation/Animation.h"
#include "../objects/SkinData.h"
#include "../objects/TextureAtlasData.h"
#include "../objects/BoneData.h"
#include "../objects/TransformFrame.h"

namespace DragonBones {

	DBFactory::DBFactory() : EventDispatcher() {
		scaleForTexture = 1;
	}

	DBFactory::~DBFactory(){
	}

	TextureAtlas* DBFactory::generateTextureAtlas(std::string imagePath, TextureAtlasData* data){
		TextureAtlas* textureAtlas = new TextureAtlas(imagePath, data);
		return textureAtlas;
	}

	Armature* DBFactory::generateArmature(){
		Armature* armature = new Armature(new DisplayObjectContainer());
		return armature;
	}

	Slot* DBFactory::generateSlot(){
		Slot* slot = new Slot(new DisplayBridge());
		return slot;
	}

	Object* DBFactory::generateDisplay(TextureAtlas* textureAtlas, std::string fullname, float pivotX, float pivotY){
		if(textureAtlas) {
			Texture* texture = textureAtlas->getTexture(fullname);
			if (texture)
			{
				Image* image = new Image(texture);
				image->setPivotX(pivotX);
				image->setPivotY(pivotY);
				image->name = fullname;
				return image;
			}
		}
		return 0;
	}

	SkeletonData* DBFactory::getSkeletonData(std::string name){
		if(_dataDic.find(name) != _dataDic.end())
		{
			SkeletonData* data = _dataDic.find(name)->second;
			if(data)
				return data;
		}
		return 0;
	}

	void DBFactory::addSkeletonData(SkeletonData* data, std::string name){
		if(name == "")
		{
			if(data)
				name = data->name;
			if(name == "")
			{
				printf("ERROR: Skeletondata has no name \n");
				return;
			}
		}
		if(data)
		{
			_dataDic.insert(std::make_pair(name,data));
			printf("Added SkeletonData: %s \n", name.c_str());
		}
		
	}

	void DBFactory::removeSkeletonData(std::string name){
		_dataDic.erase(name);
	}

	TextureAtlas* DBFactory::getTextureAtlas(std::string name){
		if(_textureAtlasDic.find(name) != _textureAtlasDic.end())
		{
			TextureAtlas* data = _textureAtlasDic.find(name)->second;
			if(data)
				return data;
		}
		return 0;
	}

	void DBFactory::addTextureAtlas(TextureAtlas* textureAtlas, std::string name){
		if(name == "")
		{
			if(textureAtlas)
				name = textureAtlas->getName();
			if(name == "")
			{
				printf("ERROR: Skeletondata has no name \n");
				return;
			}
		}
		if(textureAtlas)
		{
			_textureAtlasDic.insert(std::make_pair(name,textureAtlas));
			printf("Added TextureAtlas: %s \n", name.c_str());
		}
	}

	void DBFactory::removeTextureAtlas(std::string name){
		_textureAtlasDic.erase(name);
	}

	Armature* DBFactory::buildArmature(std::string armatureName, std::string animationName, std::string skinName, std::string skeletonName, std::string textureAtlasName){
		SkeletonData* data;
		ArmatureData* armatureData;
		if(skeletonName != "")
		{
			data = _dataDic.find(skeletonName)->second;
			if(data)
			{
				armatureData = data->getArmatureData(armatureName);
			}
		}
		else
		{
			for(std::map<std::string, SkeletonData*>::iterator it = _dataDic.begin();it!=_dataDic.end();it++)
			{
				skeletonName = it->first;
				data = it->second;
				armatureData = data->getArmatureData(armatureName);
				if(armatureData)
				{
					break;
				}
			}
		}
			
		if(!armatureData)
		{
			return 0;
		}
			
		_currentDataName = skeletonName;
		if(textureAtlasName != "") {
			//printf("TextureAtlasName: %s \n", textureAtlasName.c_str());
			_currentTextureAtlasName = textureAtlasName;
		}
		else {
			//printf("SkeletonAtlasName: %s \n", skeletonName.c_str());
			_currentTextureAtlasName = skeletonName;
		}

		Armature* armature = generateArmature();
		armature->name = armatureName;
		Bone* bone;
		for(BoneData* boneData : armatureData->_boneDataList)
		{
			bone = new Bone();
			bone->name = boneData->name;
			bone->origin->copy(boneData->transform);
			if(armatureData->getBoneData(boneData->parent))
			{
				//printf("%s with parent %s \n", bone->name.c_str(), boneData->parent.c_str());
				armature->addBone(bone, boneData->parent);
			}
			else
			{
				//printf("Bone added %s \n", bone->name.c_str());
				armature->addBone(bone);
			}

			//printf("%s   %s \n", bone->name.c_str(), bone->origin->toString().c_str());
		}
			
		ArmatureData* animationArmatureData = 0;

		if(animationName != "" && animationName != armatureName)
		{
			animationArmatureData = data->getArmatureData(animationName);
			if(!animationArmatureData)
			{
				for(std::map<std::string, SkeletonData*>::iterator it = _dataDic.begin();it!=_dataDic.end();it++)
				{
					data = it->second;
					animationArmatureData = data->getArmatureData(animationName);
					if(animationArmatureData)
					{
						break;
					}
				}
			}
		}
			
		if(animationArmatureData)
		{
			armature->getAnimation()->setAnimationDataList(animationArmatureData->getAnimationDataList());
		}
		else
		{
			armature->getAnimation()->setAnimationDataList(armatureData->getAnimationDataList());
			//printf("Size %i \n", armature->getAnimation()->getAnimationDataList().size());
		}
		
		SkinData* skinData = armatureData->getSkinData(skinName);
		Slot* slot = 0;
		DisplayData* displayData = 0;
		Armature* childArmature = 0;
		int i;
		for(SlotData* slotData : skinData->_slotDataList)
		{
			bone = armature->getBone(slotData->parent);
			if(!bone)
			{
				continue;
			}
			slot = generateSlot();
			slot->name = slotData->name;
			slot->_originZOrder = slotData->zOrder;
			slot->_displayDataList = slotData->_displayDataList;
				
			
			i = slotData->_displayDataList.size();
			std::vector<Object*> _helpArr(i);
			while(i > 0)
			{
				i--;
				displayData = slotData->_displayDataList[i];
				if(displayData->type == "armature")
				{
					childArmature = buildArmature(displayData->name, "", "", _currentDataName, _currentTextureAtlasName);
					if(childArmature)
					{
						_helpArr[i] = childArmature;
					}
							
					//trace(displayData.name, childArmature);
				} else if(displayData->type == "image")
				{
					//printf("%s %f %f \n", displayData->name.c_str(), displayData->pivot->x, displayData->pivot->y);
					//printf("%s \n", _currentTextureAtlasName.c_str());
					_helpArr[i] = generateDisplay(_textureAtlasDic[_currentTextureAtlasName], displayData->name, displayData->pivot->x, displayData->pivot->y);
				} else {
					//printf("%s \n", _currentTextureAtlasName.c_str());
					_helpArr[i] = generateDisplay(_textureAtlasDic[_currentTextureAtlasName], displayData->name, displayData->pivot->x, displayData->pivot->y);
				}
			}
			slot->setDisplayList(_helpArr);
			slot->changeDisplay(0);
			
			bone->addChild(slot);
		}
		armature->_slotsZOrderChanged = true;
		armature->advanceTime(0);
		return armature;
	}

	Object* DBFactory::getTextureDisplay(std::string textureName, std::string textureAtlasName, float pivotX, float pivotY){
		TextureAtlas* textureAtlas = 0;
		if(textureAtlasName != "")
		{
			textureAtlas = _textureAtlasDic.find(textureName)->second;
		}
		if(!textureAtlas && textureAtlasName == "")
		{
			for(std::map<std::string, TextureAtlas*>::iterator it = _textureAtlasDic.begin();it!=_textureAtlasDic.end();it++)
			{
				textureAtlas = it->second;
				if(textureAtlas->getTexture(textureName))
				{
					break;
				}
				textureAtlas=0;
			}
		}
		if(textureAtlas)
		{
			if(pivotX != pivotX || pivotY != pivotY)
			{
				SkeletonData* data = _dataDic.find(textureAtlasName)->second;
				if(data)
				{
					Vector2D* pivot = data->getSubTexturePivot(textureName);
					if(pivot)
					{
						pivotX = pivot->x;
						pivotY = pivot->y;
					}
				}
			}	
			return generateDisplay(textureAtlas, textureName, pivotX, pivotY);
		}
		return 0;
	}
}