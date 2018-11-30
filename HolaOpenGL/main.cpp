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


class Pista {
private:
	int defaultIter = 30;
	int tipo = 1; //0: sprint, 1: circuito
public:
	Vec3 puntos[MAX][2];
	float s = 20; //Escala de la cuadricula
	float d = 7.5; //Distancia desde el centro de la calle a la arista
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
		glColor3f(0.7, 0.7, 0.7);
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
				15 , mod
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
				15, mod
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

}global;

class Automovil{
private:
	GLint automovil;
	Vec3 pos = {0, 0.01, 0};
	float v = 0; //velocidad
	
				 /*
	//Trabajar mecanicas de drift/derrape
	float vA = 0; //velocidad angular
	float vAMax = 0;
	*/ 
	float n = 0.5; //Escala de las unidades
	float vMax = 70.833/FPS*n; //velocidad maxima
	float rVMax = 27.778/FPS*n; //velocidad maxima en marcha atras
	float a = 5.005/FPS*n; //Variacion de la velocidad pisando acelerador
	float aN = 0.001; //Variacion de la velociadd sin pisar el acelerador
	float aF = 0.005; //Variacion de la velocidad pisando el freno
	float rot = 180; //Rotacion alrededor del eje y
	float vRot = 2.5; //Velocidad de rotacion en angulos
public:


	void cargarAutomovil() {
		automovil = glGenLists(1);
		glNewList(automovil, GL_COMPILE);
		glPushMatrix();
		glColor3f(0.5, 0.5, 0.5);
		glScalef(5, 1.25, 2);
		glTranslatef(0, 0.5, 0);
		glutSolidCube(1);
		glPopMatrix();
		glEndList();
	}

	/*Configurar localizacion absoluta del objeto*/
	void posicionarYOrientar(Vec3 lPos, float r) {
		pos = lPos; rot = r;
	}

	/*1 = izquierda
	-1 = derecha
	*/
	void girar(int direccion) {
	
		rot += direccion * vRot;
		if (rot > 360) rot -= 360;
		else if (rot < 0) rot += 360;
	}
	void acelerar() {
		v += a;
		if (v > vMax) v = vMax;
	}

	void frenar() {
		desacelerar(aF);
	}

	void derrapar() {

	}

	void retroceder() {
		if (v > 0) desacelerar(aF);
		else{
			v -= a;
			if (v < -rVMax) v = -rVMax;
		}
	}

	void desacelerar(float aceleracion) {
		if (v > 0) {
			v -= aceleracion;
			if (v < 0) v = 0;
		}
		else if (v < 0) {
			v += aceleracion;
			if (v > 0) v = 0;
		}
	}

	Vec3 *obtenerRefPosicion() {
		return &pos;
	}

	float *obtenerRefRotacion() {
		return &rot;
	}

	void actualizar() {
		//Si nada esta ocurriendo, el auto se esta deteniendo.
		desacelerar(aN);
		pos.x +=v*cos(rot*PI/180);
		pos.z -=v*sin(rot*PI/180);
	}


	void imprimirStats(bool inicio=true) {
		char txt[]= "hola mundo";
		glPushMatrix();
		glRotatef(90, 0, 0, 1);
		cout << "Stats"<<endl;
		cout << "x: " << pos.x << "\t\t"<<endl;
		cout << "y: " << pos.y << "\t\t"<<endl;
		cout << "z: " << pos.z << "\t\t"<<endl;
		cout << "r: " << rot << "\t\t" << endl;
		cout << "v: " << v << "\t\t" << endl;
		static CONSOLE_SCREEN_BUFFER_INFO coninfo;
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsole, &coninfo);
		coninfo.dwCursorPosition.Y -= 6;    // move up one line
		coninfo.dwCursorPosition.X =0;    // move to the right the length of the word
		SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
		glPopMatrix();

	}
	void dibujarAutomovil() {
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(rot, 0, 1, 0);
		glCallList(automovil);
	}

}automovil;

class Camara {
private:
	double offsetA = 4; //Distancia en eje Y local con respecto al objeto parentado
	double offsetD = 13;  //Distancia en eje X local con respecto al objeto parentado
	double offsetAngulo = 15; //Inclinacion de la camara con respecto al eje Z local con respecto al objeto parentado

	double radio = 1; //Radio de esfera unidad
	double angulo = 35;
	double distancia = 1 / sin(angulo / 2 * PI / 180);
	double lejos = 1000; //distancia maxima que dibuja la camara
	int tipoCamara = 1; //1: Vista Planta, 2: 3ra Persona

	Vec3 *parentPos; //Posicion de referencia del objeto al que sigue (automovil)
	float *parentRot; //Rotacion de referencia del objeto al que sigue (automovil)

public:
	Vec3 pos = {0,0,5};
	Vec3 look = {0,0,0};
	Vec3 up = { 0,1,0 };

	double w = 1280/2, h = 720/2;


	void configurarTipoDeCamara(int tipo) {
		if (tipo>=1 & tipo<=2) tipoCamara = tipo;
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
	}

	void actualizarPerspectiva() {
		float ra = float(w) / float(h);
		//float d = asin(radio / sqrt(pow(x - lX, 2) + pow(y - lY, 2) + pow(z - lZ, 2)));
		//float a = 2.0 * d * 180 / PI;
		gluPerspective(angulo, ra, 0.2, lejos);
	}


	void actualizar() {
		/*Actualiza la posicion y orientacion de la camara segun
		su posicion actual
		*/
		if (tipoCamara == 1) vistaPlanta();
		else {

			pos.x = parentPos->x - offsetD * cos(*parentRot*PI / 180);
			pos.y = parentPos->y + offsetA;
			pos.z = parentPos->z + offsetD * sin(*parentRot*PI / 180);

			look.x = parentPos->x;
			look.y = parentPos->y + offsetA - tan(offsetAngulo * PI / 180)*offsetA;
			look.z = parentPos->z;
			
			
			up = Vec3(0, 1, 0);

			gluLookAt(pos.x, pos.y, pos.z,
					look.x, look.y, look.z,
						up.x, up.y, up.z
			);
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
		lejos = 100;
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
	glClearColor(1,1,1,1);
	glEnable(GL_DEPTH_TEST);

	escenario.cargarYoshi();
	automovil.cargarAutomovil();
	camara.parentarPosObjeto(automovil.obtenerRefPosicion(), automovil.obtenerRefRotacion());
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Camara
	camara.actualizar();
	
	//Escenario
	escenario.dibujarCuadricula();
	escenario.dibujarPista();
	
	//Objetos
	automovil.dibujarAutomovil();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	camara.reescalar(w,h);
	glutPostRedisplay();
}

void update() {
	if (global.obtenerEstadoTecla('w')) automovil.acelerar(); 
	if (global.obtenerEstadoTecla('s')) automovil.frenar();
	if (global.obtenerEstadoTecla('r')) automovil.retroceder();
	if (global.obtenerEstadoTecla('a')) automovil.girar(1);
	if (global.obtenerEstadoTecla('d')) automovil.girar(-1);
	if (global.obtenerEstadoTecla(' ')) automovil.derrapar();
	automovil.actualizar();
	glutPostRedisplay();
}

void onTimer(int frame) {
	update();
	//automovil.imprimirStats();
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
	glutInitWindowPosition(50, 200);

	//Tamanio de la pantalla
	glutInitWindowSize(camara.w, camara.h);

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
	//Poner en march el bucle de atencion a eventos
	glutMainLoop();
}