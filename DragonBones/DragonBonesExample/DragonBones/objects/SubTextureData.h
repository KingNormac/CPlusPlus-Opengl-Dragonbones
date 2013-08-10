#ifndef SUBTEXTUREDATA_H
#define SUBTEXTUREDATA_H

#include <string>

namespace  DragonBones {
	class SubTextureData {
	public:
		std::string name;
		int x;
		int y;
		int width;
		int height;
		SubTextureData(std::string _name, int _x, int _y, int _width, int _height) {
			name=_name;
			x=_x;
			y=_y;
			width=_width;
			height=_height;
		}
		~SubTextureData(){}
	};
}

#endif