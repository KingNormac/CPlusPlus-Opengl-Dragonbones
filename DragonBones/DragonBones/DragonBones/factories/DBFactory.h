#ifndef DBFACTORY_H
#define DBFACTORY_H

#include <string>
#include "../as3/EventDispatcher.h"

namespace DragonBones {
	class TextureAtlas;
	class Armature;
	class Slot;
	class Object;
	class Event;
	class SkeletonData;
	class TextureAtlasData;
	class DBFactory : public EventDispatcher {
	private:
		std::map<std::string, SkeletonData*> _dataDic;
		std::map<std::string, TextureAtlas*> _textureAtlasDic;
		Object* _textureAtlasLoadingDic;
		std::string _currentDataName;
		std::string _currentTextureAtlasName;
	public:
		//FROM STARLNIG
		float scaleForTexture;
		DBFactory();
		~DBFactory();
		TextureAtlas* generateTextureAtlas(std::string imagePath, TextureAtlasData* data);
		Armature* generateArmature();
		Slot* generateSlot();
		Object* generateDisplay(TextureAtlas* textureAtlas, std::string fullname, float pivotX, float pivotY);

		//FROM BASE
		SkeletonData* getSkeletonData(std::string name);
		void addSkeletonData(SkeletonData* data, std::string name="");
		void removeSkeletonData(std::string name);
		TextureAtlas* getTextureAtlas(std::string name);
		void addTextureAtlas(TextureAtlas* textureAtlas, std::string name="");
		void removeTextureAtlas(std::string name);
		Armature* buildArmature(std::string armatureName, std::string animationName="", std::string skinName="", std::string skeletonName="", std::string textureAtlasName="");
		Object* getTextureDisplay(std::string textureName, std::string textureAtlasName="", float pivotX=_Nan._Float, float pivotY=_Nan._Float);
		//void loaderCompleteHandler(Event* e);	
	};
}

#endif