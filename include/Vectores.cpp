#include "Vectores.h"
#include <math.h>
Vec2::Vec2(GLfloat X, GLfloat Y) { x = X; y = Y; }

void Vec2::imprimir(bool endLine)
{
	cout << "(" << x << ", " << y << ")";
	if (endLine) cout << endl;
}

Vec2 Vec2::operator/(float & obj)
{
	return Vec2(
		this->x / obj,
		this->y / obj);
}

Vec2 Vec2::operator*(float & obj)
{
	return Vec2(
		this->x *obj,
		this->y *obj);
}

Vec2 Vec2::operator+(Vec2 & obj)
{
	return Vec2(
		this->x + obj.x,
		this->y + obj.y);
}

Vec2 Vec2::operator-(Vec2 & obj)
{
	return Vec2(
		this->x - obj.x,
		this->y - obj.y);
}

Vec2::operator GLfloat*()
{
	
	vector[0] = x;
	vector[1] = y;
	return vector;
}

Vec3::Vec3(GLfloat X, GLfloat Y, GLfloat Z) {
	x = X; 
	y = Y; 
	z = Z; 
}

void Vec3::imprimir(bool endLine)
{
	cout << "(" << x << ", " << y << ", " << z << ")";
	if (endLine) cout << endl;
}

Vec3 Vec3::operator/(float & obj)
{
	return Vec3(
		this->x /obj,
		this->y /obj,
		this->z /obj);
}

Vec3 Vec3::operator*(float &obj)
{
	return Vec3(
		this->x * obj,
		this->y * obj,
		this->z * obj);
}

Vec3 Vec3::operator*(const int &obj)
{
	return Vec3(
		this->x * obj,
		this->y * obj,
		this->z * obj);
}

float Vec3::operator*(Vec3 & obj)
{
	return this->x*obj.x + this->y*obj.y + this->z*obj.z;
}

Vec3 Vec3::operator+(Vec3 & obj)
{
	return Vec3(
		this->x + obj.x,
		this->y + obj.y,
		this->z + obj.z);
}

Vec3 Vec3::operator+(float &obj)
{
	return Vec3(
		this->x + obj,
		this->y + obj,
		this->z + obj);
}

Vec3 Vec3::operator-(Vec3 & obj)
{
	return Vec3(
		this->x-obj.x, 
		this->y-obj.y, 
		this->z-obj.z);
}

Vec3::operator GLfloat*()
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	return vector;
}

Vec3::~Vec3(){
}

Vec4::Vec4(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W) { x = X; y = Y; z = Z; w = W; }

void Vec4::imprimir(bool endLine)
{
	cout << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	if (endLine) cout << endl;
}

Vec4 Vec4::operator/(float & obj)
{
	return Vec4(
		this->x / obj,
		this->y / obj,
		this->z / obj,
		this->w / obj);
}

Vec4 Vec4::operator*(float & obj)
{
	return Vec4(
		this->x *obj,
		this->y *obj,
		this->z *obj,
		this->w *obj);
}

Vec4 Vec4::operator+(Vec4 & obj)
{
	return Vec4(
		this->x + obj.x,
		this->y + obj.y,
		this->z + obj.z,
		this->w + obj.w);
}

Vec4 Vec4::operator-(Vec4 & obj)
{
	return Vec4(
		this->x - obj.x,
		this->y - obj.y,
		this->z - obj.z,
		this->w - obj.w);
}

Vec4::operator GLfloat*()
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;
	return vector;
}

float modulo(Vec3 fin, Vec3 in) {
	return sqrt(
		pow(fin.x - in.x, 2) +
		pow(fin.y - in.y, 2) +
		pow(fin.z - in.z, 2)
	);
}

Vec3 normalizar(Vec3 fin, Vec3 in) {
	float mod = modulo(in, fin);
	if (mod != 0)
		return (fin - in) / mod;
	else
		return Vec3(1, 0, 0);
}

float anguloRad(Vec3 fin, Vec3 in)
{
	fin = normalizar(fin);
	in = normalizar(in);
	return acos(fin*in);
}

float anguloDeg(Vec3 fin, Vec3 in) {
	return anguloRad(fin, in) * (180/PI);
}

Vec3 signoV(Vec3 v)
{
	v.x = (v.x < 0) ? -1 : 1;
	v.y = (v.y < 0) ? -1 : 1;
	v.z = (v.z < 0) ? -1 : 1;
	return v;
}
