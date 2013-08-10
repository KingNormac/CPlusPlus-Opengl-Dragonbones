#include "ColorTransform.h"

namespace DragonBones {

ColorTransform::ColorTransform() {
	this->alphaMultiplier = 1;
	this->blueMultiplier = 1;
	this->redMultiplier = 1;
	this->greenMultiplier = 1;

	this->alphaOffset = 0;
	this->blueOffset = 0;
	this->redOffset = 0;
	this->greenOffset = 0;

	this->color.alpha = 0;
	this->color.red = 0;
	this->color.green = 0;
	this->color.blue = 0;

}

void ColorTransform::setAlphaMultiplier(float value) {
	this->alphaMultiplier = value;
	this->color.alpha = this->alphaMultiplier * this->alphaOffset;
}

void ColorTransform::setBlueMultiplier(float value) {
	this->blueMultiplier = value;
	this->color.blue = this->blueMultiplier * this->blueOffset;
}

void ColorTransform::setGreenMultiplier(float value) {
	this->greenMultiplier = value;
	this->color.green = this->greenMultiplier * this->greenOffset;
}

void ColorTransform::setRedMultiplier(float value) {
	this->redMultiplier = value;
	this->color.red = this->redMultiplier * this->redOffset;
}


void ColorTransform::setAlphaOffset(float value) {
	this->alphaOffset = value;
	this->color.alpha = this->alphaMultiplier * this->alphaOffset;
}

void ColorTransform::setBlueOffset(float value) {
	this->blueOffset = value;
	this->color.blue = this->blueMultiplier * this->blueOffset;
}

void ColorTransform::setGreenOffset(float value) {
	this->greenOffset = value;
	this->color.green = this->greenMultiplier * this->greenOffset;
}

void ColorTransform::setRedOffset(float value) {
	this->redOffset = value;
	this->color.red = this->redMultiplier * this->redOffset;
}


void ColorTransform::setOffset(float red, float green, float blue, float alpha) {
	this->redOffset = red;
	this->greenOffset= green;
	this->blueOffset = blue;
	this->alphaOffset = alpha;
	this->color.alpha = this->alphaMultiplier * this->alphaOffset;
	this->color.blue = this->blueMultiplier * this->blueOffset;
	this->color.green = this->greenMultiplier * this->greenOffset;
	this->color.red = this->redMultiplier * this->redOffset;
}

void ColorTransform::setMultiplier(float red, float green, float blue, float alpha) {
	this->redMultiplier = red;
	this->greenMultiplier = green;
	this->blueMultiplier = blue;
	this->alphaMultiplier = alpha;
	this->color.alpha = this->alphaMultiplier * this->alphaOffset;
	this->color.blue = this->blueMultiplier * this->blueOffset;
	this->color.green = this->greenMultiplier * this->greenOffset;
	this->color.red = this->redMultiplier * this->redOffset;
}

ColorTransform::~ColorTransform() {

}

} /* namespace AstronusAnimation */
