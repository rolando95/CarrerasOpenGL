#include "Vectores.h"

Vec2::Vec2(GLfloat X, GLfloat Y) {x = X; y = Y;}
GLfloat *Vec2::glVec2() {
	GLfloat vector[2] = {x,y};
	return vector;
}

void Vec2::imprimir()
{
	cout << "(" << x << ", " << y << ")";
}

Vec3::Vec3(GLfloat X, GLfloat Y, GLfloat Z) { x = X; y = Y; z = Z; }
GLfloat *Vec3::glVec3() {
	GLfloat vector[3] = {x,y,z};
	return vector;
}

void Vec3::imprimir()
{
	cout << "(" << x << ", " << y << ", " << z << ")";
}

Vec4::Vec4(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W) { x = X; y = Y; z = Z; w = W; }
GLfloat *Vec4::glVec4() {
	GLfloat vector[4] = {x,y,z,w};
	return vector;
}

void Vec4::imprimir()
{
	cout << "(" << x << ", " << y << ", " << z << ", " << w << ")";
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


