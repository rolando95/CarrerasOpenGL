#pragma once
#include "Vectores.h"

class Material {
public:
	Vec4 emision;
	Vec4 ambiente;
	Vec4 difuso;
	Vec4 especular;
	Vec4 brillo;

	Material(
		Vec4 Iambiente = { 0.2, 0.2, 0.2, 1 },
		Vec4 Idifuso = { 0.8, 0.8, 0.8, 1 },
		Vec4 Iespecular = { 0.2, 0.2, 0.2, 1 },
		Vec4 Ibrillo = { 1, 1, 1, 1 },
		Vec4 Iemision = { 0,0,0,1 });
	void actualizarGlMaterialfv();
};

class Lampara : public Material {
private:
	GLfloat *tipo;
public:
	static int numLamparas; //Cuenta cuantas lamparas han sido creadas, no pasar de 8 lamparas
	int num; //Establece el numero de la lampara actual

	Vec4 posicion; //{x,y,z,tipo} (1:puntual, 0:direccional)
	Vec3 direccion; //Se aplica para las de tipo direccional
	Vec3 atenuacion = { 0.0001,0.0001,1 };
	float spotCutOff = 25;
	float spotExponent = 100;
	bool spot = false;

	//Constructor
	Lampara(
		Vec4 Iambiente = { 0.2, 0.2, 0.2, 1 },
		Vec4 Idifuso = { 0.8, 0.8, 0.8, 1 },
		Vec4 Iespecular = { 0.1, 0.1, 0.1, 1 },
		Vec4 Iposicion = { 0, 0, 0, 1 },
		Vec4 Idireccion = { 0, -1 ,0 , 0 });

	//(1:puntual, 0:direccional)
	void asignarTipo(int valor);
	void actualizarGlLightfv();
	void habilitar();
};