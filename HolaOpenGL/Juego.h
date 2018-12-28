#pragma once
#include "Propiedades.h"
#include <conio.h>
#include <ctime>
using namespace std;

constexpr auto ASCII = 256;
constexpr auto MAX = 100000;
constexpr auto FPS = 60;
constexpr auto PROYECTO = "- Maestro del derrape -";

constexpr auto urlFondoDia = "Resources/fondoDia.jpg";
constexpr auto urlFondoNoche = "Resources/fondoNoche.jpg";
constexpr auto urlReflejoFondoDia = "Resources/fondoReflejoDia.jpg";
constexpr auto urlReflejoFondoNoche = "Resources/fondoReflejoNoche.jpg";
constexpr auto urlCarretera = "Resources/carretera.jpg";
constexpr auto urlOceano = "Resources/oceano.jpg";
constexpr auto urlTerreno = "Resources/terreno.jpg";

class Pista {
private:
	float defaultIter = 30.0;
	int tipo = 1; //0: sprint, 1: circuito
	Material materialPista;
	Textura texturaPista;

	/*
	Matriz que almacena el valor de la altura de cada punto de la matriz
	Se necesita en la clase Pista para hacer los calculos necesarios
	*/
	int terreno[25][30];

public:

	Vec3 puntos[MAX][2];
	float s = 30; //Escala de la cuadricula
	float d = 8; //Distancia desde el centro de la calle a la arista

	int i;

	Pista();

	void imprimirMatrizTerreno();

	//Carga todo el circuito con forma de Yoshi
	void cargarYoshi();

	void agregarTerreno(Vec3 posicion, int valor=1);

	//Agrega una recta dado dos puntos de origen y final.
	void agregarRecta(Vec3 in, Vec3 fi, bool pushFirst = true, int iter = -1);

	/*Agrega un semicirculo en la pista
	Si el semicirculo se dibuja en el sentido contrario a las manecillas del reloj, anguloF>anguloI
	Si el semicirculo se dibujo en el sentido de las manecillas del reloj, anguloI>AnguloF
	*/
	void agregarCurva(Vec3 centro, float anguloI, float anguloF, float radio, int iter = -1, bool autoScale = true);


	//Dibujar la pista en la escena
	void dibujarPista();
};

class Escenario :public Pista{
private:
	Textura texturaFondo;
	Material materialFondo;
	GLint meshFondo;

	GLfloat costa = 10; //Distancia de la region de la cuadricula del escenario al exterior
	Textura texturaOceano;
	Material materialOceano;
	GLfloat oceanoPts[4][3] = {
		{(-14 - costa)*s,-1, costa*s},
		{ (11 + costa)*s,-1, costa*s},
		{ (11 + costa)*s,-1,(-30 - costa)*s},
		{(-14 - costa)*s,-1,(-30 - costa)*s}
	};

	Vec3 *parentPos = new Vec3;

public:
	Vec3 base[4] = {
		{ -14 * s,-0.1,0 },
		{ 11 * s,-0.1,0 },
		{ 11 * s,-0.1,-30* s },
		{ -14 * s,-0.1,-30* s }
	};

	//Recibe por referencia la posición del objeto al que el fondo siempre seguirá
	void parentarPosFondo(Vec3 *posObj);

	/*Dibuja una cuadricula base*/
	void dibujarCuadricula();

	void cargarFondo();

	/*Dibuja el fondo base*/
	void dibujarFondo();

	void dibujarTerreno();
};

class Global {
private:
	bool tecla[ASCII];
public:
	Global();

	//Asigna el estado de una tecla [char pos='w'] [bool valor=false]
	void asignarTecla(char pos, bool valor);

	//Obtiene es estado de una tecla
	bool obtenerEstadoTecla(char pos);

	/*Obtiene la direccion en memoria donde se encuentra esa tecla
	Se sugiere usar solo para lectura*/
	bool *obtenerPosTecla(char pos);
};

class Automovil {
private:
	GLint mesh;
	GLint lucesTraseras;
	Material pintura;
	Material lucesTraserasMaterial;
	Lampara lucesDelanteras;
	Textura reflejo;
	float n = 1; //Escala de las unidades

	//Movimiento Lineal
	Vec3 pos = { 0, 0.01, 0 }; //Posicion (m)
	float vL = 0; //velocidad lineal (m/s)
	float vLMax = 70.833; //velocidad maxima lineal (m/s)
	float vLMaxR = 27.778; //velocidad maxima en marcha atras (m/s)

	float aL1 = 15.001; //Variacion de la velocidad lineal pisando el acelerador en primera(m/s^2)
	float aL2 = 5.001; //Variacion de la velocidad lineal pisando el acelerador en los demas cambios (m/s^2)
	float cambio = 27.778; //Velocidad donde se realiza el cambio de primera a segunda (m/s^2)
	float aNL = 1; //Variacion de la velociad lineal sin pisar el acelerador (m/s^2)
	float aFL = 50; //Variacion de la velocidad lineal pisando el freno (m/s^2)

	//Movimiento Radial
	float rot = 180; //Rotacion alrededor del eje y (alpha)
	float vRot = 10; //Velocidad de rotacion en angulos (alpha/s)

	//Derrape (No se considera la aceleracion radial en los calculos por simplicidad)
	float rotD = 0; //Rotacion extra por derrape (alpha/s)
	float vR = 60; //Velocidad de rotacion del derrape en angulos (alpha/s)
	float vAR = 90; //Velocidad de amortiguacion de rotacion del derrape en angulos  (alpha/s)
	float rotDMax = 45; //Angulo maximo de derrape  (alpha)

	float vMinD = 27.778; //Velocidad minima para poder derrapar (m/s)

	//Puntero que guarda por referencia el estado de la pulsacion de la tecla correspondiente
	bool *parentAcelerar;
	bool *parentFreno;
	bool *parentFrenoDeMano;
	bool *parentRetroceder;
	bool *parentGiroIzquierda;
	bool *parentGiroDerecha;

public:
	//Carga la lista del modelo 3D del automovil
	void cargarAutomovil();

	/*Configurar localizacion absoluta del objeto*/
	void posicionarYOrientar(Vec3 lPos, float r);

	/*Asigna por referencia los controles del automovil*/
	void parentarControles(bool *acelerar, bool *retroceder, bool *freno, bool *frenoDeMano, bool *izquierda, bool *derecha);

	//Retorna la posicion del automovil por referencia
	Vec3 *obtenerRefPosicion();

	/*
	Retorna la rotacion del automovil por referencia
	La rotacion extra por derrape no se toma en cuenta
	*/
	float *obtenerRefRotacion();

	/*
	Rota el auto segun la direccion (-1,1)
	*/
	void girar(float direccion);

	/*
	Aumenta la velocidad segun una aceleracion dada
	*/
	void acelerar(float valor);
	/*
	Realiza la operacion inversa al metodo de aceleracion
	El argumento debe ser necesariamente de valor positivo
	*/
	void retroceder(float valor);

	/*
	Desacelera el automovil
	Toma en consideracion si su velocidad es positiva o negativa
	y automaticamente la incrementa o decrementa.
	Es necesario dar como argumento un valor de desaceleracion positivo
	*/
	void desacelerar(float valor);

	/*
	Dado un valor (positivo/negativo) aumenta o disminuye la
	rotacion extra por derrape.
	*/
	void derrapar(float valor);

	/*
	Realiza la operacion inversa del metodo derrapar
	y adicional transmite el valor de rotacion extra
	sustraido al de la rotacion del vehiculo
	*/
	void amortiguarDerrape(float valor);

	//Actualizar estado del automovil
	void actualizar();

	//Muestra los controles del automovil
	void imprimirControles();

	//Muestra el estado actual del automovil
	void imprimirStats(bool inicio = true);

	//Dibuja el automovil en la escena
	void dibujarAutomovil();

};

class Camara {
private:
	double offsetA = 4; //Distancia en eje Y local con respecto al objeto parentado
	double offsetD = 11.5;  //Distancia en eje X local con respecto al objeto parentado
	double offsetAngulo = 20; //Inclinacion de la camara con respecto al eje Z local con respecto al objeto parentado

	double radio = 1; //Radio de esfera unidad
	double angulo = 35;
	double distancia = 1 / sin(angulo / 2 * PI / 180);
	double lejos = 2000; //distancia maxima que dibuja la camara
	int tipoCamara = 2; //1: Vista Planta, 2: 3ra Persona

	Vec3 *parentPos = new Vec3; //Posicion de referencia del objeto al que sigue (automovil)
	float *parentRot = 0; //Rotacion de referencia del objeto al que sigue (automovil)

	Vec2 dimEscenario; //Dimensiones en ancho y alto del escenario para la camara en planta
	
	Vec3 pos = { 0,0,5 };
	Vec3 look = { 0,0,0 };
	Vec3 up = { 0,1,0 };
public:

	double w = 1280, h = 720;
	void configurarTipoDeCamara(int tipo);

	/*Establece el up de la camara*/
	void asignarArriba(Vec3 lUp);

	void obtenerDimensionesEscenario(float ancho, float largo);

	void reescalar(int lW = 1280, int lH = 720);

	void actualizarPerspectiva();

	void actualizar();

	/*La camara guarda por referencia la posicion
	y rotacion del objeto a seguir
	*/
	void parentarPosObjeto(Vec3 *posAuto, float *rot);

	//Situa la camara sobre la escena
	void vistaPlanta();
};

