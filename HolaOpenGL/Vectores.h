#pragma once
#include "Tools.h"

class Vec2 {
public:
	GLfloat x, y;
	Vec2(GLfloat X=0, GLfloat Y=0);
	GLfloat *glVec2();
};

class Vec3 : public Vec2 {
public:
	GLfloat z;
	Vec3(GLfloat X=0, GLfloat Y=0, GLfloat Z=0);
	GLfloat *glVec3();
};

class Vec4 : public Vec3 {
public:
	GLfloat w;
	Vec4(GLfloat X=0, GLfloat Y=0, GLfloat Z=0, GLfloat W=0);
	GLfloat *glVec4();
};


