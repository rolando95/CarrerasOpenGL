#pragma once
#include "Vectores.h"

constexpr auto urlDigitos = "Resources/digitos.png";
constexpr auto urlLetras = "Resources/letras.png";
constexpr auto urlOceano2 = "Resources/oceano.jpg";

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
	float spotExponent = 30; //Distribución de intensidad cerca de los bordes. Valores entre [0,128]

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
	void inhabilitar();
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

class Plano2D {
public:
	Plano2D(float x0 = -1,float y0 = -1,float x1 = 1, float y1 = 1);
};

class Plano2DTex {
public:
	Plano2DTex(float xmin=0, float ymin=0, float xmax=1, float ymax=1, float x0 = -1, float y0 = -1, float x1 = 1, float y1 = 1);
};

static class Texto {
public:
	Textura letras, digitos, oceano;
	void cargarImagenes();
	/*
	Dibuja un numero en la escena.
	Si se define numero de digitos, se dibujará ceros a la izquierda.
	numDigitos=0 para dibujar el numero solamente.
	AlineacionX (Alineacion del texto horizontal) 1->left, 0 ->center, -1 ->right
	AlineacionY (Alineacion del texto vertical) 1->top, 0 ->middle, -1 ->bottom
	*/
	void entero(int n, int numDigitos=0, int alineacionX=1 , int alineacionY=1);
	
	/*
	Dibuja una palabra en la escena.
	AlineacionX (Alineacion del texto horizontal) 1->left, 0 ->center, -1 ->right
	AlineacionY (Alineacion del texto vertical) 1->top, 0 ->middle, -1 ->bottom
	*/
	void palabra(const char *palabra, int alineacionX=1, int alineacionY=1);
};