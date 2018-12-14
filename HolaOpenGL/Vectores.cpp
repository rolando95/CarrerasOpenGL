#include "Vectores.h"

Vec2::Vec2(GLfloat X, GLfloat Y) {x = X; y = Y;}
GLfloat *Vec2::glVec2() {
	GLfloat vector[2];
	vector[0] = x;
	vector[1] = y;
	return vector;
}

Vec3::Vec3(GLfloat X, GLfloat Y, GLfloat Z) { x = X; y = Y; z = Z; }
GLfloat *Vec3::glVec3() {
	GLfloat vector[3];
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	return vector;
}

Vec4::Vec4(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W) { x = X; y = Y; z = Z; w = W; }
GLfloat *Vec4::glVec4() {
	GLfloat vector[4];
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;
	return vector;
}

float modulo(Vec3 in, Vec3 fi) {
	return sqrt(
		pow(fi.x - in.x, 2) +
		pow(fi.y - in.y, 2) +
		pow(fi.z - in.z, 2)
	);
}

Vec3 normalizar(Vec3 in, Vec3 fi) {
	float mod = modulo(in, fi);
	return Vec3(
		(fi.x - in.x) / mod,
		(fi.y - in.y) / mod,
		(fi.z - in.z) / mod
	);
}


