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
		Vec4 Iambiente = { 0.8, 0.8, 0.8, 1 },
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

	float spotCutOff = 45; //Angulo de dispersión. Valores entre [0,90] o 128.
	float spotExponent = 15; //Distribución de intensidad cerca de los bordes. Valores entre [0,128]

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

class Textura {
private:
	GLuint textura;
	int uv = 1; //1:Normal, 2:Reflexion
public:
	/*Carga la textura dada la ruta de ubicacion*/
	void cargarTextura(const char *ruta);

	//1:Normal, 2:Reflexion
	void asignarTipoUV(int tipo);

	/*Actualiza textura*/
	void actualizar();
};

/*
Algoritmo de 4-conectada
*matriz -> Matriz de N filas y M columnas

Se recomienda enviar como parametro &matriz[0][0] o (int *)matriz

N -> filas
M -> columnas
x -> fila semilla
y -> columna semilla
viejo -> valor viejo para sobreescribir
nuevo -> valor nuevo
*/
void conecta4(int *matriz, int N, int M, int x, int y,int viejo, int  nuevo);
