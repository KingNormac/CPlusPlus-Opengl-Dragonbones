#ifndef TEXTUREATLASDATA_H
#define TEXTUREATLASDATA_H

#include <string>
#include <vector>
#include "SubTextureData.h"

namespace DragonBones {
	class TextureAtlasData {
	private:
		std::vector<SubTextureData*> _subTextureDataList;
	public:
		std::string name;
		int width;
		int height;

		TextureAtlasData(std::string _name) {
			name=_name;
			width=0;
			height=0;
		}

		TextureAtlasData(std::string _name, int _width, int _height) {
			name=_name;
			width=_width;
			height=_height;
		}

		~TextureAtlasData() {
			for(int i=0;i<_subTextureDataList.size();i++) {
				SubTextureData* data = _subTextureDataList[i];
				if(data) {
					delete[] data;
				}
			}
			_subTextureDataList.clear();
		}

		void addSubTextureData(SubTextureData* data) {
			_subTextureDataList.push_back(data);
		}

		SubTextureData* getSubTextureData(std::string name) {
			for(int i=0;i<_subTextureDataList.size();i++) {
				SubTextureData* data = _subTextureDataList[i];
				if(data) {
					if(data->name == name) {
						return data;
					}
				}
			}
			return 0;
		}

		std::vector<SubTextureData*> getSubTextureDataList(){return _subTextureDataList;}
	};
}

#endif