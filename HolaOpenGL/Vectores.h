
#pragma once
#include "Tools.h"



class Vec2 {
public:
	GLfloat x, y;
	Vec2(GLfloat X=0, GLfloat Y=0);
	
	//Muestra por pantalla el vector (x,y)
	void imprimir();

	//template<typename Type>
	Vec2 operator/(float &obj);
	Vec2 operator*(float &obj);
	Vec2 operator+(Vec2 &obj);
	Vec2 operator-(Vec2 &obj);
	operator GLfloat*();
};

class Vec3 : public Vec2 {
public:
	GLfloat z;
	Vec3(GLfloat X=0, GLfloat Y=0, GLfloat Z=0);
	
	//Muestra por pantalla el vector (x,y,z)
	void imprimir();

	Vec3 operator/(float &obj);


	Vec3 operator*(float &obj);
	Vec3 operator*(const int &obj);
	float operator*(Vec3 &obj);
	Vec3 operator+(Vec3 &obj);
	Vec3 operator+(float &obj);
	Vec3 operator-(Vec3 &obj);
	operator GLfloat*();
};

class Vec4 : public Vec3 {
public:
	GLfloat w;
	Vec4(GLfloat X=0, GLfloat Y=0, GLfloat Z=0, GLfloat W=0);
	

	//Muestra por pantalla el vector (x,y,z,w)
	void imprimir();

	Vec4 operator/(float &obj);
	Vec4 operator*(float &obj);
	Vec4 operator+(Vec4 &obj);
	Vec4 operator-(Vec4 &obj);
	operator GLfloat*();
};

/*Calcula el modulo de un vector de 3 dimensiones
Si es dado por un punto inicial y final,  asignar in e fi.
Si solo es un valor vectorial, asignar in.
*/
float modulo(Vec3 fin, Vec3 in = { 0,0,0 });

//Normaliza un vector dado un vector de 3 dimensiones o dos puntos {x,y,z}
Vec3 normalizar(Vec3 fin, Vec3 in = { 0,0,0 });

//Devuelve el angulo entre 2 Vec3
float anguloRad(Vec3 fin, Vec3 in = { 0,0,0 });

//Devuelve el angulo entre 2 Vec3
float anguloDeg(Vec3 fin, Vec3 in = { 0,0,0 });

//Devuelve 1 y -1 segun el signo de cada componente
Vec3 signoV(Vec3 vector);


