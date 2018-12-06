/*
	ProyectoFinal

	Juego de carreras con mucho drift!!

	@author: Rolando A. Rosales J.
	@version: v1.0 x/dic/2018
	@require: freeglut

*/

#include <iostream>
#include "gl/freeglut.h"
#include <utilidades.h>
#include <ctime>
#include <math.h>
#define ASCII 256
#define MAX 100000
#define FPS 60
#define PROYECTO "- Drift Racing -"

using namespace std;

class Vec2 {
public:
	GLfloat x, y;
	Vec2(GLfloat X = 0, GLfloat Y = 0) {
		x = X; y = Y;
	}

	GLfloat *glVec2() {
		GLfloat vector[2];
		vector[0] = x;
		vector[1] = y;
		return vector;
	}
};

class Vec3: public Vec2 {
public:
	GLfloat z;
	Vec3(GLfloat X=0, GLfloat Y=0, GLfloat Z=0) {
		x = X; y = Y; z = Z;
	}
	GLfloat *glVec3() {
		GLfloat vector[3];
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		return vector;
	}
};

class Vec4 : public Vec3 {
public:
	GLfloat w;
	Vec4(GLfloat X = 0, GLfloat Y = 0, GLfloat Z = 0, GLfloat W = 0) {
		x = X; y = Y; z = Z; w = W;
	}
	GLfloat *glVec4() {
		GLfloat vector[4];
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		vector[3] = w;
		return vector;
	}
};


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
		Vec4 Ibrillo = {1, 1, 1, 1},
		Vec4 Iemision = { 0,0,0,1 }) {
		ambiente = Iambiente;
		difuso = Idifuso;
		especular = Iespecular;
		brillo = Ibrillo;
		emision = Iemision;
	}
	void actualizarGlMaterialfv() {
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente.glVec4());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso.glVec4());
		glMaterialfv(GL_FRONT, GL_EMISSION, emision.glVec4());
		glMaterialfv(GL_FRONT, GL_SHININESS, brillo.glVec4());
		glMaterialfv(GL_FRONT, GL_SPECULAR, especular.glVec4());
	}
}baseMaterial;

class Lampara: public Material {
public:
	static int numLamparas; //Cuenta cuantas lamparas han sido creadas, no pasar de 8 lamparas
	int num; //Establece el numero de la lampara actual
	
	Vec4 posicion; //{x,y,z,tipo} (1:puntual, 0:direccional)
	Vec3 direccion; //Se aplica para las de tipo direccional
	Vec3 atenuacion = { 0.0001,0.0001,1};
	GLfloat *tipo;

	float spotCutOff = 25;
	float spotExponent = 100;
	bool spot = false;
	Lampara(
		Vec4 Iambiente = {0.2, 0.2, 0.2, 1},
		Vec4 Idifuso = {0.8, 0.8, 0.8, 1},
		Vec4 Iespecular = {0.1, 0.1, 0.1, 1},
		Vec4 Iposicion = { 0, 0, 0, 1 },
		Vec4 Idireccion = { 0, -1 ,0 , 0 }){
		direccion = Idireccion;
		posicion = Iposicion;
		ambiente = Iambiente;
		difuso = Idifuso;
		especular = Iespecular;

		tipo = &posicion.w;
		num = numLamparas;
		numLamparas++;
	}

	//(1:puntual, 0:direccional)
	void asignarTipo(int valor) {
		*tipo = valor;
	}


	void actualizarGlLightfv() {
		glLightfv(GL_LIGHT0 + num, GL_AMBIENT, ambiente.glVec4());
		glLightfv(GL_LIGHT0 + num, GL_DIFFUSE, difuso.glVec4());
		glLightfv(GL_LIGHT0 + num, GL_SPECULAR, especular.glVec4());
		glLightfv(GL_LIGHT0 + num, GL_POSITION, posicion.glVec4());
		if (*tipo == 1){
			if(spot){
				glLightf(GL_LIGHT0 + num, GL_SPOT_CUTOFF, spotCutOff);
				glLightf(GL_LIGHT0 + num, GL_SPOT_EXPONENT, spotExponent);
				glLightfv(GL_LIGHT0 + num, GL_SPOT_DIRECTION, direccion.glVec3());
			}
			glLightfv(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, atenuacion.glVec3());
		}
		
	}
	void habilitar() {
		glEnable(GL_LIGHT0 + num);
	}
};
//Inicializa el conteo de lamparas
int Lampara::numLamparas = 0;
Lampara luzAmbiente;

class Pista {
private:
	float defaultIter = 30.0;
	int tipo = 1; //0: sprint, 1: circuito
	Material materialPista;
public:
	Vec3 puntos[MAX][2];
	float s = 30; //Escala de la cuadricula
	float d = 8; //Distancia desde el centro de la calle a la arista
	int i;

	/*Calcula el modulo de un vector 
	Si es dado por un punto inicial y final,  asignar in e fi.
	Si solo es un valor vectorial, asignar in.
	*/
	float modulo(Vec3 in, Vec3 fi = {0,0,0}) {
		return sqrt(
			pow(fi.x - in.x, 2) +
			pow(fi.y - in.y, 2) +
			pow(fi.z - in.z, 2)
		);
	}

	Vec3 vectorUnidad(Vec3 in, Vec3 fi) {
		float mod = modulo(in, fi);
		return Vec3(
			(fi.x-in.x)/mod,
			(fi.y-in.y)/mod,
			(fi.z-in.z)/mod
		);
	}

	/*Agrega una recta dado dos puntos de origen y final.
	pushFirst es un argumento especial que indica si el punto inicial se toma en cuenta
	Al hacer la llamada, la instruccion anterior de agregar fue
	Agregar curva cerrada, se recomienda asignar pushFirst=false o
	dar un el inicial con un offset en direccion a la tangente
	*/
	void agregarRecta(Vec3 in, Vec3 fi, bool pushFirst=true, bool pushLast=false, int iter = -1) {
		in = { in.x*s, in.y*s, in.z*s };
		fi = { fi.x*s, fi.y*s, fi.z*s };

		if (iter < 1) iter = modulo(in, fi)/defaultIter;
		if (iter < 2) iter = 2;

		float cambio = modulo(in,fi) / iter;
		Vec3 unidad = vectorUnidad(in, fi);
		Vec3 inf(
			in.x + unidad.z*d,
			in.y,
			in.z - unidad.x*d
		);

		Vec3 sup(
			in.x - unidad.z*d,
			in.y,
			in.z + unidad.x*d
		);
		
		if (pushLast) iter++;

		for (auto j = 0; j < iter; j++) {
			if(pushFirst || j>0){
				puntos[i][0] = inf;
				puntos[i++][1] = sup;
			}
			
			sup.x += unidad.x*cambio;
			sup.z += unidad.z*cambio;

			inf.x += unidad.x*cambio;
			inf.z += unidad.z*cambio;
		}
	}
	
	/*Agrega un semicirculo en la pista
	Si el semicirculo se dibuja en el sentido contrario a las manecillas del reloj, anguloF>anguloI
	Si el semicirculo se dibujo en el sentido de las manecillas del reloj, anguloI>AnguloF
	*/
	void agregarCurvaAbierta(Vec3 centro, float anguloI, float anguloF, float radio, int iter=-1, bool autoScale = true) {
		if (autoScale) {
			centro = { centro.x*s, centro.y*s, centro.z*s };
			radio *= s;
		}

		if (iter < 1) iter = abs(anguloF - anguloI) / defaultIter* 2 *((radio/s < 1) ? 1 : radio / s);

		float cambio = (anguloF - anguloI)*PI/180 / iter;

		
		float angulo = anguloI * PI / 180;
		int n = 0;
		if (anguloF > anguloI) n = 1;

		for (auto j = 0; j < iter; j++) {
			puntos[i][n] = {
				centro.x + radio * cos(angulo) + d * cos(angulo),
				centro.y,
				centro.z - radio * sin(angulo) - d * sin(angulo),
			};
			puntos[i++][1-n] = {
				centro.x + radio * cos(angulo) - d * cos(angulo),
				centro.y,
				centro.z - radio * sin(angulo) + d * sin(angulo),
			};
			angulo += cambio;
		}
	}

	void cargarYoshi() {

		materialPista.difuso = Vec4(0.3, 0.3, 0.3, 1);

		i = 0;
		float j;
		//Planta del pie
		agregarRecta(Vec3(4, 0, 0), Vec3(-4, 0, 0));

		//Punta del pie
		agregarCurvaAbierta(Vec3(-4, 0, -2.5), 270, 90, 2.5);

		//Tobillo
		agregarCurvaAbierta(Vec3(-4, 0, -6), -90, 60, 1);

		//Barriga
		//agregarRecta(Vec3(-4, 0, -7), Vec3(-5, 0, -8));
		agregarCurvaAbierta(Vec3(-6, 0, -7), 60, 90, 2);

		//Brazo
		agregarRecta(Vec3(-6, 0, -9), Vec3(-8, 0, -9));
		agregarCurvaAbierta(Vec3(-8, 0, -10.5), 270,90, 1.5);
		agregarCurvaAbierta(Vec3(-8, 0, -13), -90, -15, 1);
		
		//Cuello (adelante)
		agregarRecta(Vec3(-7, 0, -13), Vec3(-6.5, 0, -14.9));

		//Nariz
		agregarCurvaAbierta(Vec3(-7.5, 0, -15), -15, 90, 1);
		agregarRecta(Vec3(-7.5, 0, -16), Vec3(-10, 0, -16));
		agregarCurvaAbierta(Vec3(-10, 0, -19), 270, 90, 3);
		agregarRecta(Vec3(-10, 0, -22), Vec3(-9, 0, -22));
		agregarCurvaAbierta(Vec3(-9, 0, -24), -90, 0, 2);

		//Ojos
		agregarCurvaAbierta(Vec3(-4.5, 0, -24), 180, 0, 2.5);
		agregarCurvaAbierta(Vec3(-1, 0, -24), 180, 270, 1);
		agregarCurvaAbierta(Vec3(-1, 0, -27), 270, 285, 4);

		//Nuca
		agregarCurvaAbierta(Vec3(1, 0, -22), 115, -45, sqrt(2));
		agregarCurvaAbierta(Vec3(2.25, 0, -20.5), 135, 225, 0.5);
		agregarCurvaAbierta(Vec3(1, 0, -19), 45, -45, sqrt(2));
		agregarCurvaAbierta(Vec3(2.25, 0, -17.5), 135, 225, 0.5);
		agregarCurvaAbierta(Vec3(1, 0, -16), 45, -45, sqrt(2));
		agregarRecta(Vec3(1.925, 0, -15), Vec3(-0.25, 0, -14));
		agregarCurvaAbierta(Vec3(0, 0, -13.5), 135, 270, 0.5);

		//Espalda
		agregarRecta(Vec3(0, 0, -13), Vec3(3, 0, -13));
		agregarCurvaAbierta(Vec3(3, 0, -10), 90, 75, 3);
		agregarCurvaAbierta(Vec3(6, 0, -15), -120, -60, 3);
		agregarCurvaAbierta(Vec3(9, 0, -10), 105, 90, 3);

		//Cola
		agregarCurvaAbierta(Vec3(9,0,-12),90,-45,1);
		agregarRecta(Vec3(7, 0, -9), Vec3(5, 0, -6));
		agregarCurvaAbierta(Vec3(4.5, 0, -5.5), 135, 270, 0.5);

		//Talon
		agregarCurvaAbierta(Vec3(4, 0, -2.5), 75, -75, 2.5);


	}

	void dibujarPista() {
		glPushMatrix();
		glPushAttrib(GL_FRONT_AND_BACK);
		materialPista.actualizarGlMaterialfv();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		float mod;
		for (auto j = 0; j < i - 1; j++) {
			mod = modulo(puntos[j][0], puntos[j + 1][0]);
			if (mod < 1) mod = 1;
			quad(
				puntos[j][0].glVec3(),
				puntos[j][1].glVec3(),
				puntos[j + 1][1].glVec3(),
				puntos[j + 1][0].glVec3(),
				15 , (int)mod
			);
		}
		if (tipo == 1) {
			mod = modulo(puntos[i-1][0], puntos[0][0]);
			if (mod < 1) mod = 1;
			quad(
				puntos[i - 1][0].glVec3(),
				puntos[i - 1][1].glVec3(),
				puntos[0][1].glVec3(),
				puntos[0][0].glVec3(),
				15, (int)mod
			);
		}
		glPopAttrib();
		glPopMatrix();
	}
};

class Escenario :public Pista{
public:
	Vec3 base[4] = {
		{ -15*s,-0.1,0 },
		{ 11*s,-0.1,0 },
		{ 11*s,-0.1,-28*s },
		{ -15*s,-0.1,-28*s }
	};

	/*Dibuja una cuadricula base*/
	void dibujarCuadricula() {
		//cuadricula base
		glPushMatrix();
		glTranslatef(0, 0.1, 0);
		glScalef(10, 10, 10);
		ejes();
		glPopMatrix();
		glPushAttrib(GL_FRONT_AND_BACK);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.9, 0.9, 0.9);
		
		//utilidades.h
		quad(base[0].glVec3(),
			base[1].glVec3(),
			base[2].glVec3(),
			base[3].glVec3(), 
			25, 
			30
		);

		glPopAttrib();
	}
}escenario;

class Global {
private:
	bool tecla[ASCII];
public:
	Global() {
		for (auto i = 0; i < ASCII; i++) {
			tecla[i] = false;
		}
	}
	void asignarTecla(char pos, bool valor) {
		tecla[pos] = valor;
	}

	bool obtenerEstadoTecla(char pos) {
		return tecla[pos];
	}

	bool *obtenerPosTecla(char pos) {
		return &tecla[pos];
	}
}global;

class Automovil{
private:
	GLint mesh;
	GLint lucesTraseras;
	Material pintura;
	Material lucesTraserasMaterial;
	Lampara lucesDelanteras;
	float n = 1; //Escala de las unidades

	//Movimiento Lineal
	Vec3 pos = {0, 0.01, 0}; //Posicion (m)
	float vL = 0; //velocidad lineal (m/s)
	float vLMax = 70.833; //velocidad maxima lineal (m/s)
	float vLMaxR = 27.778; //velocidad maxima en marcha atras (m/s)

	float aL1 = 15.001; //Variacion de la velocidad lineal pisando el acelerador en primera(m/s^2)
	float aL2 = 5.001; //Variacion de la velocidad lineal pisando el acelerador en los demas cambios (m/s^2)
	float cambio = 27.778; //Velocidad donde se realiza el cambio de primera a segunda (m/s^2)
	float aNL = 0.008; //Variacion de la velociad lineal sin pisar el acelerador (m/s^2)
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
	void cargarAutomovil() {
		pintura.difuso = Vec4(1,0,0,1);
		pintura.especular = Vec4(1, 1, 1, 1);
		pintura.brillo = Vec4(75, 75, 75, 1);

		mesh = glGenLists(1);
		glNewList(mesh, GL_COMPILE);
		glPushMatrix();
		glScalef(5, 1.75, 2);
		glTranslatef(0, 0.5, 0);
		glutSolidCube(1);
		glPopMatrix();
		glEndList();

		lucesTraserasMaterial.difuso = Vec4(0.1, 0, 0, 1);
		lucesTraserasMaterial.especular = Vec4(.5, .25, .25, 1);
		lucesTraseras = glGenLists(1);
		glNewList(lucesTraseras, GL_COMPILE);
		glPushMatrix();
		glTranslatef(-2.5, 0.85, -0.5);
		glScalef(0.1, 0.18, 0.35);
		glutSolidCube(1);
		glPopMatrix();
		glEndList();

		lucesDelanteras.difuso = Vec4(1, 1, 1, 1);
		lucesDelanteras.ambiente = Vec4(1, 1, 1, 1);
		lucesDelanteras.posicion = Vec4(-3.5, 1, 0, 1);
		lucesDelanteras.especular = Vec4(1, 1, 1, 1);
		lucesDelanteras.asignarTipo(1);
		lucesDelanteras.spot = true;
		lucesDelanteras.direccion = Vec3(1, 0, 0);
		lucesDelanteras.habilitar();
	}

	/*Configurar localizacion absoluta del objeto*/
	void posicionarYOrientar(Vec3 lPos, float r) {
		pos = lPos; rot = r;
	}

	/*Obtener por referencia 
	los valores de entrada de los controles del automovil
	*/
	void parentarControles(bool *acelerar, bool *retroceder, bool *freno, bool *frenoDeMano, bool *izquierda, bool *derecha) {
		parentAcelerar = acelerar;
		parentFreno = freno;
		parentFrenoDeMano = frenoDeMano;
		parentGiroIzquierda = izquierda;
		parentGiroDerecha = derecha;
		parentRetroceder = retroceder;
	}

	//Retorna la posicion del automovil por referencia
	Vec3 *obtenerRefPosicion() {
		return &pos;
	}

	/*
	Retorna la rotacion del automovil por referencia
	La rotacion extra por derrape no se toma en cuenta
	*/
	float *obtenerRefRotacion() {
		return &rot;
	}

	/*
	Rota el auto segun la direccion (-1,1)
	*/
	void girar(float direccion) {
		if(vL != 0){
			//En velocidad negativa se invierte la transmision 
			//del giro acorde a los autos reales
			if (vL < 0) direccion *= -1; 
			float val = 1 / cosh((abs(vL) - cambio)*PI / 180);
			rot += direccion * vRot / FPS / 2 * val;
			if (rot > 360) rot -= 360;
			else if (rot < 0) rot += 360;
		}
	}

	/*
	Aumenta la velocidad segun una aceleracion dada
	*/
	void acelerar(float valor) {
		//abs(valor);
		vL += valor/FPS;
		if (vL > vLMax) vL = vLMax;
	}

	/*
	Realiza la operacion inversa al metodo de aceleracion
	El argumento debe ser necesariamente de valor positivo
	*/
	void retroceder(float valor) {
		//abs(valor);
		vL -= valor/FPS;
		if (vL < -vLMaxR) vL = -vLMaxR;
	}

	/*
	Desacelera el automovil
	Toma en consideracion si su velocidad es positiva o negativa
	y automaticamente la incrementa o decrementa.
	Es necesario dar como argumento un valor de desaceleracion positivo
	*/
	void desacelerar(float valor) {
		//abs(valor);
		if (vL > 0) {
			vL -= valor/FPS;
			if (vL < 0) vL = 0;
		}
		else if (vL < 0) {
			vL += valor/FPS;
			if (vL > 0) vL = 0;
		}
	}

	/*
	Dado un valor (positivo/negativo) aumenta o disminuye la
	rotacion extra por derrape.
	*/
	void derrapar(float valor) {
		if (abs(vL) > vMinD)
			rotD += valor/FPS;
		else
			rotD += valor/FPS * abs(vL) / vMinD;
		if (rotD > rotDMax) rotD = rotDMax;
		else if (rotD < -rotDMax) rotD = -rotDMax;

	}

	/*
	Realiza la operacion inversa del metodo derrapar
	y adicional transmite el valor de rotacion extra
	sustraido al de la rotacion del vehiculo
	*/
	void amortiguarDerrape(float valor) {
		if (rotD != 0) {
			float reduccion = 0;
			if (rotD > 0) {
				rotD -= valor/FPS;
				if (rotD < 0) {
					reduccion = valor/FPS - abs(rotD);
					rotD = 0;
				}
				else reduccion = valor/FPS;
			}
			else if (rotD < 0) {
				rotD += valor/FPS;
				if (rotD > 0) {
					reduccion = -valor/FPS + rotD;
					rotD = 0;
				}
				else reduccion = -valor/FPS;
			}

			//El valor de rotacion extra reducido se
			//transmite a rot
			rot += reduccion/2;
		}
	}

	//Actualizar estado del automovil
	void actualizar() {

		//Velocidad Lineal
		if (*parentFreno) desacelerar(aFL);
		else if (*parentAcelerar) {
			if(vL > cambio)acelerar(aL2);
			else acelerar(aL1);
			if (*parentFrenoDeMano) desacelerar(aNL);
		}
		else if (*parentRetroceder && !*parentFrenoDeMano) retroceder(aL2);
		else desacelerar(aNL);

		//Velocidad Angular
		if (!*parentGiroIzquierda && *parentGiroDerecha) {
			if (*parentFrenoDeMano) derrapar(-vR);
			girar(-vRot);
		}
		if (*parentGiroIzquierda && !*parentGiroDerecha) {
			if (*parentFrenoDeMano) derrapar(vR);
			girar(vRot);
		}

		if (!*parentFrenoDeMano) amortiguarDerrape(vAR);
		else amortiguarDerrape(vAR/25);

		//Actualizacion de posicion
		pos.x += vL * cos((rot-rotD/2)*PI / 180)/FPS;
		pos.z -= vL * sin((rot-rotD/2)*PI / 180)/FPS;
	}

	void imprimirStats(bool inicio = true) {
		int i = 0;

		static int anterior = 0;

		int ahora = (int)time(0);
		static int conteo = 0;
		static int fps = 0;
		conteo += 1;
		if (ahora - anterior > 1) {
			fps = conteo;
			conteo = 0;
			anterior = ahora-1;
		}
		cout << "Estado del Automovil" << endl; i++;
		cout << "______________________\n" << endl; i+=2;
		cout << "Pos x: " << round(pos.x*100)/100 << " m\t\t" << endl; i++;
		cout << "Pos y: " << round(pos.y*100)/100 << " m\t\t" << endl; i++;
		cout << "Pos z: " << round(pos.z*100)/100 << " m\t\t" << endl; i++;
		cout << "Velocidad: " << vL *3600/1000 << " km/h \t\t" << endl; i++;
		cout << "Rotacion : " << rot << " grados\t\t" << endl; i++;
		cout << "Rot derrape: " << rotD << " grados\t\t" << endl; i++;
		cout << "Acelerar: " << *parentAcelerar << "\t\t" << endl; i++;
		cout << "Giro iz : " << *parentGiroIzquierda << "\t\t" << endl; i++;
		cout << "Giro der: " << *parentGiroDerecha << "\t\t" << endl; i++;
		cout << "Freno   : " << *parentFreno<< "\t\t" << endl; i++;
		cout << "Retroceder : " << *parentRetroceder << "\t\t" << endl; i++;
		cout << "FrenoMano  : " << *parentFrenoDeMano<< "\t\t" << endl; i++;
		cout << "FPS: " << fps << "\t\t" << endl; i++;
		static CONSOLE_SCREEN_BUFFER_INFO coninfo;
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsole, &coninfo);
		coninfo.dwCursorPosition.Y -= i;    // move up one line
		coninfo.dwCursorPosition.X =0;    // move to the right the length of the word
		SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
	}
	void dibujarAutomovil() {
		glPushMatrix();
		glPushAttrib(GL_FRONT);
		
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(rot+rotD, 0, 1, 0);
		
		pintura.actualizarGlMaterialfv();
		glCallList(mesh);

		glPushMatrix();
		if (*parentFrenoDeMano || *parentFreno || *parentRetroceder){
			lucesTraserasMaterial.emision = Vec4(1, 0, 0, 1);
		}
		else
			lucesTraserasMaterial.emision = Vec4(0.1, 0, 0, 1);
		lucesTraserasMaterial.actualizarGlMaterialfv();
		glCallList(lucesTraseras);
		glScalef(1, 1, -1);
		glCallList(lucesTraseras);
		glPopMatrix();

		lucesDelanteras.actualizarGlLightfv();

		glPopAttrib();
		glPopMatrix();
	}

}automovil;

class Camara {
private:
	double offsetA = 4; //Distancia en eje Y local con respecto al objeto parentado
	double offsetD = 11.5;  //Distancia en eje X local con respecto al objeto parentado
	double offsetAngulo = 20; //Inclinacion de la camara con respecto al eje Z local con respecto al objeto parentado

	double radio = 1; //Radio de esfera unidad
	double angulo = 35;
	double distancia = 1 / sin(angulo / 2 * PI / 180);
	double lejos = 1000; //distancia maxima que dibuja la camara
	int tipoCamara = 2; //1: Vista Planta, 2: 3ra Persona

	Vec3 *parentPos; //Posicion de referencia del objeto al que sigue (automovil)
	float *parentRot; //Rotacion de referencia del objeto al que sigue (automovil)

public:
	Vec3 pos = {0,0,5};
	Vec3 look = {0,0,0};
	Vec3 up = { 0,1,0 };

	double w = 1280, h = 720;


	void configurarTipoDeCamara(int tipo) {
		if (tipo>=1 && tipo<=2) tipoCamara = tipo;
	}

	/*Establece punto de posicion de la camara
	(No usar este metodo si se trabaja con parentarPosObject)*/
	void posicionar(Vec3 lPos) {
		pos = lPos;
	}

	/*Establece orientacion de la camara dando punto de mira
	(No usar este metodo si se trabaja con parentarPosObject)*/
	void orientar(Vec3 lLook) {
		look = lLook;
	}

	void asignarArriba(Vec3 lUp) {
		/*Establece el up de la camara
		(es necesario llamar localicar para actualizar en la escena)*/
		up = lUp;
	}


	void reescalar(int lW=1280, int lH=720) {
		w = lW; h = lH;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		actualizarPerspectiva();
		glutPostRedisplay();
	}

	void actualizarPerspectiva() {
		float ra = float(w) / float(h);
		gluPerspective(angulo, ra, 0.2, lejos);
	}


	void actualizar() {
		/*Actualiza la posicion y orientacion de la camara segun
		su posicion actual
		*/
		if (tipoCamara == 1) vistaPlanta();
		else {

			/*
			Se posiciona la camara antes de hacer 
			de hacer el calculo del frame actual.
			Crea un ligero retraso que mejorar la jugabilidad
			*/
			gluLookAt(pos.x, pos.y, pos.z,
				look.x, look.y, look.z,
				up.x, up.y, up.z
			);

			//Calculo de la nueva posicion de camara
			pos.x = parentPos->x - offsetD * cos(*parentRot*PI / 180);
			pos.y = parentPos->y + offsetA;
			pos.z = parentPos->z + offsetD * sin(*parentRot*PI / 180);

			look.x = parentPos->x;
			look.y = parentPos->y + offsetA - tan(offsetAngulo * PI / 180)*offsetA;
			look.z = parentPos->z;
			
			
			up = Vec3(0, 1, 0);
			
			/*
			gluLookAt(pos.x, pos.y, pos.z,
				look.x, look.y, look.z,
				up.x, up.y, up.z
			); */
		}
	}

	/*La camara guarda por referencia la posicion
	y rotacion del objeto a seguir
	*/
	void parentarPosObjeto(Vec3 *posAuto, float *rot) { 
		parentPos = posAuto;
		parentRot = rot; //Rotacion de referencia del objeto al que sigue (automovil)
	}

	void vistaPlanta() {
		/*
		Vista especial para mostrar todo el mapa del juego
		(Solo para uso de testing)
		*/
		radio = 14;
		lejos = 10000;
		float x = escenario.base[3].x - escenario.base[0].x;
		float y = abs(escenario.base[2].z - escenario.base[1].z)/2;
		float z = -y;
		posicionar(Vec3(x, y/sin(angulo/2*PI/180), z));
		orientar(Vec3(0, 0, z));
		asignarArriba(Vec3(0, 0, -1));
		gluLookAt(pos.x, pos.y, pos.z,
			look.x, look.y, look.z,
			up.x, up.y, up.z
		);
	}
}camara;


double alpha=0;

void init() {
	//Fijar color de borrado
	glClearColor(0,0,0.2,1);
	
	glEnable(GL_DEPTH_TEST);

	//Iluminacion
	glEnable(GL_LIGHTING);

	luzAmbiente.posicion = Vec4(1, 1, 0,0);
	luzAmbiente.direccion = Vec3(0, -1, 0);
	luzAmbiente.ambiente = Vec4(0.1,0.1,0.1, 1);
	luzAmbiente.asignarTipo(0);
	luzAmbiente.difuso = Vec4(.2, .2, .2, 1);
	luzAmbiente.especular = Vec4(0, 0, 0, 1);
	luzAmbiente.habilitar();

	escenario.cargarYoshi();

	bool *acelerar = global.obtenerPosTecla('w');
	bool *retroceder = global.obtenerPosTecla('r');
	bool *freno = global.obtenerPosTecla('s');
	bool *frenoDeMano = global.obtenerPosTecla(' ');
	bool *izquierda = global.obtenerPosTecla('a');
	bool *derecha = global.obtenerPosTecla('d');
	automovil.parentarControles(acelerar, retroceder, freno, frenoDeMano, izquierda, derecha);
	automovil.cargarAutomovil();

	camara.parentarPosObjeto(automovil.obtenerRefPosicion(), automovil.obtenerRefRotacion());
	camara.actualizar();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Camara
	camara.actualizar();


	glPushMatrix();
	glPushAttrib(GL_FRONT);
	
	{

		luzAmbiente.actualizarGlLightfv();
		
		//Material base
		baseMaterial.actualizarGlMaterialfv();

		//Escenario
		escenario.dibujarCuadricula();
		escenario.dibujarPista();

		//Objetos
		automovil.dibujarAutomovil();
		glutSwapBuffers();
	}
	glPopMatrix();
	glPopAttrib();
	automovil.imprimirStats();
}

void reshape(int w, int h) {
	camara.reescalar(w,h);
}

void update() {
	automovil.actualizar();
	glutPostRedisplay();
	
}

void onTimer(int frame) {
	update();
	glutTimerFunc(frame, onTimer, frame);
}

void onKey(unsigned char tecla, int x, int y) {
	global.asignarTecla(tecla, true);
	if (tecla-48 >= 0 && tecla-48 <= 9) {
		camara.configurarTipoDeCamara(tecla - 48);
	}
	if (tecla == 27) exit(0); //27: Esc
}
void onUpKey(unsigned char tecla, int x, int y) {
	global.asignarTecla(tecla, false);
}

void onSpecialKey(int specialKey, int x, int y) {
	if (specialKey == GLUT_KEY_UP) global.asignarTecla('w',true); 
	if (specialKey == GLUT_KEY_DOWN) global.asignarTecla('s', true);
	if (specialKey == GLUT_KEY_RIGHT) global.asignarTecla('d', true);
	if (specialKey == GLUT_KEY_LEFT) global.asignarTecla('a', true);
}

void onSpecialUpKey(int specialKey, int x, int y) {
	if (specialKey == GLUT_KEY_UP) global.asignarTecla('w', false);
	if (specialKey == GLUT_KEY_DOWN) global.asignarTecla('s', false);
	if (specialKey == GLUT_KEY_RIGHT) global.asignarTecla('d', false);
	if (specialKey == GLUT_KEY_LEFT) global.asignarTecla('a', false);
}
void onClick(int button, int state, int x, int y) {

}

void onDrag(int x, int y) {
}

void onMove(int x, int y) {
}



void main(int argc, char **argv) {


	glutInit(&argc, argv);
	
	//Buffers a usar
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//Posicionar la ventana de dibujo en la pantalla
	glutInitWindowPosition(0, 0);

	//Tamanio de la pantalla
	glutInitWindowSize((int)camara.w,(int)camara.h);

	//Crear la ventana
	glutCreateWindow(PROYECTO);
	init();
	//Registrar las callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey);
	glutKeyboardUpFunc(onUpKey);
	glutSpecialFunc(onSpecialKey);
	glutSpecialUpFunc(onSpecialUpKey);
	glutMouseFunc(onClick); //Click de mouse
	glutMotionFunc(onDrag); //Movimiento del mouse con algun boton presionado
	glutPassiveMotionFunc(onMove); //Movimiento del mouse sin necesidad de boton presionado
	glutTimerFunc(1000/FPS, onTimer, 1000/FPS);
	glutMainLoop();
}