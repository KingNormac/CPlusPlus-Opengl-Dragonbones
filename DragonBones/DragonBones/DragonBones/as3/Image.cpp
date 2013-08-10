#include "Image.h"
#include "Texture.h"
#include "../utils/TransformUtil.h"
#include "DisplayObjectContainer.h"

namespace DragonBones {
	Image::Image(Texture* tex) : DisplayObject(){
		texture = tex;
		visible = true;
	}

	void Image::render() {
		if(visible) {
			if(texture) {

				if(parent)
				{
					texture->render(this->parent->rec*getTransformationMatrix());
				} else {

					texture->render(getTransformationMatrix());

				}

			}
		}
	}
}