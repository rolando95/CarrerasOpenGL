#pragma once
#include "Tools.h"
class Vec2 {
public:
	GLfloat x, y;
	Vec2();
	Vec2(GLfloat X, GLfloat Y);
	GLfloat *glVec2();
};

class Vec3 : public Vec2 {
public:
	GLfloat z;
	Vec3();
	Vec3(GLfloat X, GLfloat Y, GLfloat Z);
	GLfloat *glVec3();
};

class Vec4 : public Vec3 {
public:
	GLfloat w;
	Vec4();
	Vec4(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W);
	GLfloat *glVec4();
};


