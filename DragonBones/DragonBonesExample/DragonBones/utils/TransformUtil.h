#ifndef TRANSFORMUTIL_H
#define TRANSFORMUTIL_H

#include "../objects/DBTransform.h"
#include <math.h>
#include "../glm/glm.hpp"

namespace DragonBones {
	namespace TranformUtils {

	glm::mat4 transformToMatrix(DBTransform* transform);
	float formatRadian(float radian);

	void transformPointWithParent(DBTransform* transform, DBTransform* parent);

	void printMatrix(glm::mat4 matrix);

	glm::mat4 pivotMatrix(glm::mat4 mat, float x, float y);
}
}

#endif