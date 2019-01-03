#include "Vectores.h"

Vec2::Vec2(GLfloat X, GLfloat Y) { x = X; y = Y; }

void Vec2::imprimir()
{
	cout << "(" << x << ", " << y << ")";
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
	GLfloat vector[2] = { x,y };
	return vector;
}

Vec3::Vec3(GLfloat X, GLfloat Y, GLfloat Z) { x = X; y = Y; z = Z; }

void Vec3::imprimir()
{
	cout << "(" << x << ", " << y << ", " << z << ")";
}

Vec3 Vec3::operator/(float & obj)
{
	return Vec3(
		this->x /obj,
		this->y /obj,
		this->z /obj);
}

Vec3 Vec3::operator*(float & obj)
{
	return Vec3(
		this->x *obj,
		this->y *obj,
		this->z *obj);
}

Vec3 Vec3::operator+(Vec3 & obj)
{
	return Vec3(
		this->x + obj.x,
		this->y + obj.y,
		this->z + obj.z);
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
	GLfloat vector[3] = { x,y,z };
	return vector;
}


Vec4::Vec4(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W) { x = X; y = Y; z = Z; w = W; }

void Vec4::imprimir()
{
	cout << "(" << x << ", " << y << ", " << z << ", " << w << ")";
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
	GLfloat vector[4] = { x,y,z,w };
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


