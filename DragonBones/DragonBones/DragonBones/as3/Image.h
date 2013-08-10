#ifndef TEXTURE_H
#define TEXTURE_H

#include "DisplayObject.h"
#include "../as3/ColorTransform.h"

class Texture;

namespace DragonBones {
	class Image : public DisplayObject {
	public:
		Texture* texture;

		Image(Texture* tex);
		void render();
		ColorTransform* color;
	};
}

#endif