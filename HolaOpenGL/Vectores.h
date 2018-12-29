#pragma once
#include "Tools.h"

class Vec2 {
public:
	GLfloat x, y;
	Vec2(GLfloat X=0, GLfloat Y=0);
	GLfloat *glVec2();
	void imprimir();
};

class Vec3 : public Vec2 {
public:
	GLfloat z;
	Vec3(GLfloat X=0, GLfloat Y=0, GLfloat Z=0);
	GLfloat *glVec3();
	void imprimir();
};

class Vec4 : public Vec3 {
public:
	GLfloat w;
	Vec4(GLfloat X=0, GLfloat Y=0, GLfloat Z=0, GLfloat W=0);
	GLfloat *glVec4();
	void imprimir();
};

/*Calcula el modulo de un vector de 3 dimensiones
Si es dado por un punto inicial y final,  asignar in e fi.
Si solo es un valor vectorial, asignar in.
*/
float modulo(Vec3 in, Vec3 fi = { 0,0,0 });

//Normaliza un vector dado un vector de 3 dimensiones o dos puntos {x,y,z}
Vec3 normalizar(Vec3 in, Vec3 fi);


