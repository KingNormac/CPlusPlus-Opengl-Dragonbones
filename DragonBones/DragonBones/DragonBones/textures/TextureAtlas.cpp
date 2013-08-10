#include "TextureAtlas.h"
#include "../objects/TextureAtlasData.h"
#include "../as3/Texture.h"

namespace DragonBones {

	std::string TextureAtlas::getName(){
		return _name;
	}

	TextureAtlas::TextureAtlas(std::string imagePath, TextureAtlasData* data){
		_scale = 1;
		this->_subTextureDic.clear();
		_imagepath = imagePath;
		parseData(data);
		this->_name = data->name;
	}


	TextureAtlas::~TextureAtlas(){
	}

	Texture* TextureAtlas::getTexture(std::string name){
		if(_subTextureDic.find(name) != _subTextureDic.end())
		{
			Texture* found = _subTextureDic.find(name)->second;
			if(found)
				return found;
		}
		return 0;
	}

	void TextureAtlas::parseData(TextureAtlasData* data){
		_name = data->name;
		for(SubTextureData* sData : data->getSubTextureDataList())
		{
			Texture* subTexture = new Texture(_imagepath.c_str(), sData->x, sData->y, sData->width, sData->height);
			if(subTexture) {
				this->_subTextureDic.insert(std::make_pair(sData->name, subTexture));
			}
		}
	}

}