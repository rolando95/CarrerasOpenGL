#pragma once
#include "Propiedades.h"
#include <conio.h>
#include <ctime>

using namespace std;

//Distancia de dibujado en la escena
constexpr auto distanciaDibujado = 500;
constexpr auto distanciaDibujadoHight = distanciaDibujado / 3;

//Escala de la escena y entorno
constexpr auto s = 50; 

constexpr auto ASCII = 256;
constexpr auto MAX = 1000;
constexpr auto FPS = 60;
constexpr auto PROYECTO = "- Maestros del Derrape - Por: Rolando A. Rosales J.";

//Texturas
constexpr auto urlBlank = "Resources/blank.jpg";

constexpr auto urlFondoDia = "Resources/fondoDia.jpg";
constexpr auto urlFondoNoche = "Resources/fondoNoche.jpg";
constexpr auto urlReflejoFondoDia = "Resources/fondoReflejoDia.jpg";
constexpr auto urlReflejoFondoNoche = "Resources/fondoReflejoNoche.jpg";
constexpr auto urlCarretera = "Resources/carretera.jpg";
constexpr auto urlOceano = "Resources/oceano.jpg";
constexpr auto urlTerreno = "Resources/terreno.jpg";
constexpr auto urlAcantilado = "Resources/acantilado.jpg";
constexpr auto urlAcantilado2 = "Resources/acantilado2.jpg";

constexpr auto urlCircuitoMapa = "Resources/circuitoMapa.png";
constexpr auto urlAutomovilPosicionMapa = "Resources/automovilPosicionMapa.png";
constexpr auto urlVelocimetroInterior = "Resources/velocimetroInterior.png";
constexpr auto urlVelocimetroExterior = "Resources/velocimetroExterior.png";
constexpr auto urlBanderaMeta = "Resources/banderaMeta.jpg";

constexpr auto urlVistaHelicoptero = "Resources/circuitoVistaHelicoptero.png";

class Interfaz {
	Textura circuitoMapa, posAutoMapa, velocimetroInterior, velocimetroExterior;
	Texto texto;

	Vec2 *parentResolucion = new Vec2; //Resolucion de la ventana por referencia
	Vec3 *parentPos = new Vec3; //Posicion de referencia del objeto al que sigue (automovil)
	float *parentRot = 0; //Rotacion de referencia del objeto al que sigue (automovil)
	float *parentVelocidad = 0; //Velocidad de referencia del objeto al que sigue (automovil)
	float parentMaxV=-100, parentMaxVR=300; //Maxima velocidad hacia el frente y en reversa

	void ajustarProporcionPantalla();
public:
	void cargarInterfaz();

	/*La interfaz guarda por referencia la posicion
	y rotacion del objeto para mostrar en el mapa*/
	void parentarPosObjeto(Vec3 *posAuto, float *rot,float *velocidad,float maximaVelocidadReversa, float maximaVelocidad);
	void parentarResolucionVentana(Vec2 *resolucion);
	void dibujarInterfaz();
	void dibujarPausa();

	//Muestra el estado actual del automovil
	void imprimirStats(bool inicio = true);
};

class Global {
private:
	bool tecla[ASCII]; //Estado de pulsaci�n de la tecla (true->pulsada, false->soltada)
	bool flipTecla[ASCII]; //Cada vez que se pulse una tecla se cambia de estado
	
	/*
	Trabajar con teclas min�sculas. 
	Por defecto las may�sculas le�das se convertir�n en min�sculas
	*/
	char pausa = 'p';
	char modoSolido = 'm';
	char interfaz = 'c';
	char horario = 'l'; 
	char niebla = 'n';
	char captura = 'v';
	
	//Capturas de pantalla
	const char urlFichero[20] = "Capturas/screenshot";
	char urlImagen[27];
	int numCaptura = 0;

	//0->Noche, 1->Dia
	Vec4 difusoColor[2] = { Vec4(.05, .05, .05, 1), Vec4(.4, .4, .41, 1) };
	Vec4 ambienteColor[2] = { Vec4(0.02, 0.02, 0.02, 1), Vec4(0.1,0.1,0.1,1)};

	Vec4 fondoEmisionColor[2] = { Vec4(0.4,0.4,0.4,1), Vec4(1,1,1,1) };

	Textura texturaFondo[2];

	Material materialFondo;
	GLint meshFondo;

	GLfloat costa = 10; //Distancia de la region de la cuadricula del escenario al exterior
	Textura texturaOceano;
	Material materialOceano;

	Vec3 oceanoPts[4] = {
		{(-14 - costa)*s,-10, costa*s},
		{ (11 + costa)*s,-10, costa*s},
		{ (11 + costa)*s,-10,(-30 - costa)*s},
		{(-14 - costa)*s,-10,(-30 - costa)*s}
	};

	//Posicion por referencia del objeto a seguir
	Vec3 *parentPos = new Vec3;

	//Resolucion de pantalla
	Vec2 *parentResolucion = new Vec2;

	//Almacena los segundos transcurridos desde el inicio del juego. Requiere actualizarse llamando la funcion actualizar
	float tiempo = 0;

public:
	Lampara luzAmbiente;
	Global();

	//Devuelve los segundos transcurridos desde el inicio del juego
	float obtenerTiempo();

	void actualizar();

	//Recibe por referencia la posici�n del objeto al que el fondo siempre seguir�
	void parentarPosFondo(Vec3 *posObj);
	
	//Recibe por referencia la resolucion de la pantalla
	void parentarResolucionVentana(Vec2 *resolucion);

	void cargarConfiguracionesGlobales();

	void actualizarConfiguracionesGlobales();

	//Asigna el estado de una tecla [char pos='w'] [bool valor=false]
	void asignarTecla(char pos, bool valor);

	//Obtiene el estado de una tecla
	bool obtenerEstadoTecla(char pos);

	//Obtiene el estado de una tecla flip
	bool obtenerEstadoTeclaFlip(char pos);

	//Obtiene el estado del horario en el juego
	bool obtenerHorario();

	//Obtiene el estado de la pausa en el juego
	bool obtenerPausa();

	//Obtiene el estado de la interfaz en el juego
	bool obtenerInterfaz();

	/*Obtiene la direccion en memoria donde se encuentra esa tecla
	Se sugiere usar solo para lectura*/
	bool *obtenerPosTecla(char pos);

	/*Carga el fondo y el mar de la escena*/
	void cargarFondo();

	/*Dibuja el fondo en la escena*/
	void dibujarFondo();

	/*Dibuja el mar
	tipo: 1-> mesh en la escena, 2-> textura de fondo*/
	void dibujarMar(bool detalleBajo=false);

	void imprimirControles();
};

class Pista {
private:

	float aT = 1.5; //Ancho del terreno
	float aC = 2.5; //Ancho de la colina/mar (se cuenta desde la orilla de la carretera)
	float base = 0.1;
	float altura = 20; //Altura de la colina/ profundidad de la costa

	float defaultIter = 30.0; //Resolucion de la pista (mas es mejor, afecta la resolucion del terreno tambien)

	//Agrega una recta dado dos puntos de origen y final.
	void agregarRecta(Vec3 in, Vec3 fi, bool pushFirst = true, int iter = -1);

	/*Agrega un semicirculo en la pista
	Si el semicirculo se dibuja en el sentido contrario a las manecillas del reloj, anguloF>anguloI
	Si el semicirculo se dibujo en el sentido de las manecillas del reloj, anguloI>AnguloF
	*/
	void agregarCurva(Vec3 centro, float anguloI, float anguloF, float radio, int iter = -1, bool autoScale = true);

public:
	int tipo = 1; //0: sprint, 1: circuito
	Material materialPista;
	Textura texturaPista;
	Textura vistaHelicoptero;
	Material materialTerreno;
	Textura texturaTerreno[3];

	//Puntos de la pista
	Vec3 puntos[MAX][2];
	int i; //numero de puntos de la pista

	/*Puntos del terreno 
	terreno[n][0] = posicion mar
	terreno[n][1] = inicio terreno
	terreno[n][2] = fin terreno
	terreno[n][3] = posicion colina*/
	Vec3 terreno[MAX][4]; 
	int resCurva = 3; //Resolucion del terreno en curva //Menos es mas detallado
	int resRect = 2;// Resolucion del terreno en rectas //Menos es mas detallado
	int ti; //numero de puntos del terreno


	Vec3 farolas[MAX];
	float angulofi[MAX];
	float distFi = 5 * s;
	float antFi;
	int fi;


	float d = 8; //Distancia desde el centro de la calle a la arista


public:
	//Carga todo el circuito (Tiene forma similar al Yoshi Circuit de Mario Kart, aunque con gr�ficos de N64)
	void cargarPista();
};

class Escenario :public Pista{
	Vec3 *parentPos = new Vec3;

	GLint meshFarola;
	Material materialFarola;
	Lampara lamparasFarolas[3];
	int numLamparasFarolas = 3;

	GLint meshMeta;
	Material materialMetaPilares;
	Material materialMetaParteSuperior;
	Textura texturaMetaParteSuperior;
	Textura texturaBlank;
public:
	float cercaFi = 0; //Posicion del terreno m�s cercana al automovil (util para agilizar c�lculos en colisiones)

	//Limites de la cuadricula
	Vec3 base[4] = {
		{ -15 * s,-0.1,0 },
		{  15 * s,-0.1,0 },
		{  15 * s,-0.1,-30* s },
		{ -15 * s,-0.1,-30* s }
	};

	void asignarEstadoFarolas(int valor);

	void cargarAssets();

	//Recibe por referencia la posici�n del objeto al que el fondo siempre seguir�
	void parentarPosFondo(Vec3 *posObj);

	/*Dibuja una cuadricula base*/
	void dibujarCuadricula();

	/*
	Dibujar la pista en la escena tomando en cuenta la distancia de dibujado.
	Se puede activar el forzarDibujarTodo para siempre mostrar toda la pista*/
	void dibujarPista(bool forzarDibujarTodo=false);

	/*Dibujar el terreno en la escena tomando en cuenta la distancia de dibujado
	Se puede configurar el detalleAlto para dibujar todo lo visible con bajo detalle
	Se puede activar el forzarDibujarTodo para siempre mostrar toda la pista
	*/
	void dibujarTerreno(bool detalleBajo = false, bool forzarDibujarTodo=false);

	/*Dibujar las farolas en la escena tomando en cuenta la distancia e dibujado.*/
	void dibujarAssets(bool forzarDibujarTodo = false);

	void dibujarTexturaVistaHelicoptero();
};

class Automovil {
private:
	GLint mesh;
	GLint lucesTraseras;
	Lampara lucesDelanteras;
	Material pintura;
	Material mateGris;
	Material mateNegro;
	Material lucesTraserasMaterial;
	Textura reflejo;
	
	/*Escala de velocidad lineal 
	Ayuda a mejorar la sensacion de velocidad en la escenas, mas no afecta en los calculos f�sicos
	*/
	float escalaVelocidadL = 2;

	//Movimiento Lineal
	Vec3 pos; //Posicion (m)
	float vL = 0; //velocidad lineal (m/s)
	float vLMax = 70.833; //velocidad maxima lineal (m/s)
	float vLMaxR = 27.778; //velocidad maxima en marcha atras (m/s)

	float aL1 = 5.001; //Variacion de la velocidad lineal pisando el acelerador en primera(m/s^2)
	float aL2 = 3.5; //Variacion de la velocidad lineal pisando el acelerador en segunda (m/s^2)
	float cambio = 27.778; //Velocidad donde se realiza el cambio de primera a segunda (m/s^2)
	float aNL = 1; //Variacion de la velociad lineal sin pisar el acelerador (m/s^2)
	float aFL = 25; //Variacion de la velocidad lineal pisando el freno (m/s^2)
	float coefDrift = 1.05; //Porcentaje artificial de reduccion de velocidad cuando se presiona el freno de mano (x100%)

	//Movimiento Radial
	float rot = 0; //Rotacion alrededor del eje y (alpha)
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

public:

	//Carga la lista del modelo 3D del automovil
	void cargarAutomovil();

	/*Configurar localizacion absoluta del objeto*/
	void posicionarYOrientar(Vec3 lPos, float r);

	/*Asigna por referencia los controles del automovil*/
	void parentarControles(bool *acelerar, bool *retroceder, bool *freno, bool *frenoDeMano, bool *izquierda, bool *derecha);

	//Retorna rango de velocidad lineal
	Vec2 obtenerRangoVelocidadLineal();

	//Retorna la posicion del automovil por referencia
	Vec3 *obtenerRefPosicion();
	
	/*
	Retorna la rotacion del automovil por referencia
	La rotacion extra por derrape no se toma en cuenta
	*/
	float *obtenerRefRotacion();

	//Retorna la rotacion extra por derrape del automovil por referencia
	float *obtenerRefRotacionExtra();

	float *obtenerRefVelocidad();

	//Actualizar estado del automovil
	void actualizar();

	//Dibuja el automovil en la escena
	void dibujarAutomovil();

	//Activa/Desactiva las luces delanteras (true->habilitado, false->inhabilitado)
	void asignarEstadoLucesDelanteras(bool valor);
};

class Camara {
private:
	double offsetA = 4; //Distancia en eje Y local con respecto al objeto parentado en 3ra persona
	double offsetAFPS = 2; //Distancia en eje Y local con respecto al objeto parentado en 1ra persona
	double offsetD = 11.5;  //Distancia en eje X local con respecto al objeto parentado
	double offsetAngulo = 20; //Inclinacion de la camara con respecto al eje Z local con respecto al objeto parentado

	double radio = 1; //Radio de esfera unidad
	double angulo = 35;
	double distancia = 1 / sin(angulo / 2 * PI / 180);
	double lejos = 50*s; //distancia maxima que dibuja la camara
	int tipoCamara = 2; //1->Camara Helicoptero, 2-> 3ra Persona, 3-> 1ra Persona

	Automovil *automovil;

	Vec3 *parentPos = new Vec3; //Posicion de referencia del objeto al que sigue (automovil)
	float *parentExtraRot = new float(0); //Rotacion de referencia del objeto al que sigue (derrape del automovil)
	float *parentRot = 0; //Rotacion de referencia del objeto al que sigue (automovil)

	Vec2 dimEscenario; //Dimensiones en ancho y alto del escenario para la camara en planta
	
	Vec3 pos = { 0,0,5 };
	Vec3 look = { 0,0,0 };
	Vec3 up = { 0,1,0 };
public:

	Vec2 resolucion = { 1280, 720 };
	void configurarTipoDeCamara(int tipo);

	/*Establece el up de la camara*/
	void asignarArriba(Vec3 lUp);

	void asignarDimensionesEscenario(float ancho, float largo);

	void reescalar(int lW = 1280, int lH = 720);

	int obtenerTipoCamara();

	void actualizarPerspectiva();

	//Retorna la posicion de la camara por referencia
	Vec3 *obtenerRefPosicion();

	//Retorna la resolucion de el pantalla por referencia
	Vec2 *obtenerRefResolucion();

	void actualizar();


	void parentAutomovil(Automovil *obj);

	/*La camara guarda por referencia la posicion
	y rotacion del objeto a seguir
	*/
	void parentarPosObjeto(Vec3 *posAuto, float *rot);

	/*La camara guarda por referencia la rotacion
	extra por derrape del objeto a seguir
	*/
	void parentarExtraRotObjeto(float *extraRot);

	//Situa la camara sobre la escena
	void vistaPlanta();
};

/*
Colisionador bastante sencillo.
Funciona utilizando el plano XZ.
Solo tiene como utilidad detectar, no realiza ning�n c�lculo f�sico
*/
class Colisiones2D {
	Vec3 terreno[MAX][4];
	int ti;

	float distR = 3; //Distancia entre los dos radios del automovil
	float radio = 1; //Radio de la base de la circunferencia de colision
	float offset = 0;

	float rGrande = radio * 2 + distR;

	Vec3 *parentPos = new Vec3;
	float *parentRot = new float(0);
	float *parentExtraRot = new float(0);
	float *parentVelocidad = new float(0);
public:
	//Recibe los puntos del terreno para mas adelante realizar los c�lculos de detecci�n de colisiones
	void obtenerPuntosTerreno(Vec3 terreno[MAX][4], int ti);

	/*
	Recibe los radios y la distancia entre ellos para la colision del automovil
	radio -> radio de las circunferencias de colision
	distanciaEntreRadios -> distancia en el eje X entre las circunferencias
	offset -> La colision supone que ambas circunferencias estan centradas en el origen si offset=0,
	con valor distinto se desplazan en el eje X relativo al obj
	*/
	void asignarMeshColision(float radio);

	void parentarObjeto(Vec3 *objPos, float *objRot, float *objVelocidad);
	/*
	pistaTerreno -> Valor ti m�s cercano al automovil para no tener que hacer todo el c�lculo de b�squeda de nuevo
	*/


	/*La camara guarda por referencia la rotacion
	extra por derrape del objeto a seguir
	*/
	void parentarExtraRotObjeto(float *extraRot);

	void colisionar(int pistaTerreno = -1);
};