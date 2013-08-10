#include "TransformUtil.h"

namespace DragonBones {
	namespace TranformUtils {
	const double PI = atan(1.0)*4;
	const double HALF_PI = PI*0.5f;
	const double DOUBLE_PI = PI*2;

	void transformPointWithParent(DBTransform* transform, DBTransform* parent) {
		glm::mat4 _helpMatrix = transformToMatrix(parent);
		_helpMatrix = glm::inverse(_helpMatrix);

		float x = transform->x;
		float y = transform->y;

		//transform.x = _helpMatrix.a * x + _helpMatrix.c * y + _helpMatrix.tx;
		//transform.y = _helpMatrix.d * y + _helpMatrix.b * x + _helpMatrix.ty;

		transform->x = _helpMatrix[0][0] * x + _helpMatrix[1][0] * y + _helpMatrix[3][0];
		transform->y = _helpMatrix[1][1] * y + _helpMatrix[0][1] * x + _helpMatrix[3][1];

		transform->skewX = formatRadian(transform->skewX - parent->skewX);
		transform->skewY = formatRadian(transform->skewY - parent->skewY);

	}

	glm::mat4 transformToMatrix(DBTransform* transform) {
		float a =   transform->scaleX * cos(transform->skewY);
		float b =   transform->scaleX * sin(transform->skewY);
		float c =  -transform->scaleY * sin(transform->skewX);
		float d =   transform->scaleY * cos(transform->skewX);
		float tx =  transform->x;
		float ty =  transform->y;

		return glm::mat4(a,b,0,0,c,d,0,0,0,0,1,0,tx,ty,0,1);
	}

	float formatRadian(float radian) {
		float rad = fmod(radian, DOUBLE_PI);
		if (rad > PI) {
			rad -= DOUBLE_PI;
		}
		if (rad < -PI) {
			rad += DOUBLE_PI;
		}
		//printf("%f \n ", rad);
		return rad;
	}

	void printMatrix(glm::mat4 matrix) {
		printf("A: %f ", matrix[0][0]); //a
		printf("B: %f ", matrix[0][1]); //b
		printf("C: %f ", matrix[1][0]); //c
		printf("D: %f ", matrix[1][1]); //d
		printf("TX: %f ", matrix[3][0]); //tx
		printf("TY: %f \n", matrix[3][1]); //ty
	}

	glm::mat4 pivotMatrix(glm::mat4 mat, float x, float y) {
		glm::mat4 matrix = mat;
		matrix[3][0] -= matrix[0][0] * x + matrix[1][0] * y;
		matrix[3][1] -= matrix[0][1] * x + matrix[1][1] * y;
		return matrix;
		//matrix.tx -= matrix.a * pivotX + matrix.c * pivotY;
		//matrix.ty -= matrix.b * pivotX + matrix.d * pivotY;
	}
	}
}