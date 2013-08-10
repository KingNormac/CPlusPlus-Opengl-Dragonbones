#ifndef COLORTRANSFORM_H_
#define COLORTRANSFORM_H_

namespace DragonBones {

struct Color {
	float red, blue, green, alpha;
};

class ColorTransform {
public:
	ColorTransform();
	~ColorTransform();

	void setAlphaMultiplier(float value);
	void setBlueMultiplier(float value);
	void setGreenMultiplier(float value);
	void setRedMultiplier(float value);

	void setAlphaOffset(float value);
	void setBlueOffset(float value);
	void setGreenOffset(float value);
	void setRedOffset(float value);

	void setOffset(float red, float green, float blue, float alpha);
	void setMultiplier(float red, float green, float blue, float alpha);

	Color getColor(){return color;}

	float alphaMultiplier;
	float alphaOffset;
	float blueMultiplier;
	float blueOffset;
	Color color;
	float greenMultiplier;
	float greenOffset;
	float redMultiplier;
	float redOffset;
};

} /* namespace AstronusAnimation */
#endif
