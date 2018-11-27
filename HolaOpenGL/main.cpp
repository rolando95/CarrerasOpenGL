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
#include <map>
//#include <stdio.h>
//#include <iostream>
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

	GLfloat *glVec3() {
		GLfloat vector[3];
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

class Quad {
public:
	Vec3 q1, q2, q3, q4;
	Quad(Vec3 Q1 = { 0,0,0 }, 
		 Vec3 Q2 = { 0,0,0 }, 
		 Vec3 Q3 = { 0,0,0 }, 
		 Vec3 Q4 = { 0,0,0 })
	{
		q1 = Q1; q2 = Q2; q3 = Q3; q4 = Q4;
	}

	//Devuelve la distancia absoluta entre las coordenadas x de las q
	float distX() {
		return 0;
	}
	float distY() {
		return 0;
	}
	float distZ() {
		return 0;
	}

};


class Pista {
	Quad quads[MAX];
}pista;

class Escenario {
public:
	Quad base = {
		{ -14,0,0 },
		{ 11,0,0 },
		{ 11,0,-30 },
		{ -14,0,-30 }
	};

	/*Cuadricula base*/
	void dibujarCuadricula() {
		//Cuadricula base
		ejes();
		glPushAttrib(GL_FRONT_AND_BACK);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.9, 0.9, 0.9);
		
		//utilidades.h
		quad(base.q1.glVec3(),
			base.q2.glVec3(),
			base.q3.glVec3(),
			base.q4.glVec3(), 
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

class Jugador{
private:
	GLint automovil;
	Vec3 pos = { 0, 0.01, -0.25 };
	float v = 0; //velocidad
	/*
	//Trabajar mecanicas de drift/derrape
	float vA = 0; //velocidad angular
	float vAMax = 0;
	*/ 

	float vMax = 0.5; //velocidad maxima
	float rVMax = 0.05; //velocidad maxima en reversa
	float a = 0.002; //Variacion de la velocidad pisando acelerador
	float aN = 0.001; //Variacion de la velociadd sin pisar el acelerador
	float aF = 0.005; //Variacion de la velocidad pisando el freno
	float rot = 180; //Rotacion alrededor del eje y
	float vRot = 1; //Velocidad de rotacion en angulos
public:


	void cargarAutomovil() {
		automovil = glGenLists(1);
		glNewList(automovil, GL_COMPILE);
		glPushMatrix();
		glColor3f(0.5, 0.5, 0.5);
		glScalef(0.5, 0.01, 0.5);
		glutSolidCube(0.5);
		glTranslatef(0.25, 0, 0);
		glColor3f(1, 0, 0);
		glutSolidSphere(0.25,5,5);
		glPopMatrix();
		glEndList();
	}

	void posicionarYOrientar(Vec3 lPos, float r) {
		/*Posicion absoluta del objeto*/
		pos = lPos; rot = r;
	}

	void girar(int direccion) {
	/*1 = derecha
	 -1 = izquierda
	*/
		if(v!=0){
			rot += direccion * vRot;
			if (rot > 360) rot -= 360;
			else if (rot < 0) rot += 360;
		}
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

	Vec3 obtenerPosicion() {
		return pos;
	}

	float obtenerRotacion() {
		return rot;
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

}jugador;

class Camara {
private:

	double lejos = 100;
	double offsetA = .3;
	double offsetD = 1.25;  //distancia con respecto al objeto parentado

	double radio = 1; //Radio de esfera unidad
	double angulo = 35;
	double distancia = 1 / sin(angulo / 2 * PI / 180);
	int tipoCamara = 2; //1: Vista Planta, 2:3ra Persona

public:
	Vec3 pos = {0,0,5};
	Vec3 look = {0,0,0};
	Vec3 up = { 0,1,0 };

	double w = 1280/2, h = 720/2;


	void configurarTipoDeCamara(int tipo) {
		tipoCamara = tipo;
	}

	void posicionar(Vec3 lPos) {
		/*Establece punto de posicion de la camara
		(es necesario llamar localicar para actualizar en la escena)*/
		pos = lPos;
	}

	void orientar(Vec3 lLook) {
		/*Establece orientacion de la camara dando punto de mira
		(es necesario llamar localicar para actualizar en la escena)*/
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
			gluLookAt(pos.x, pos.y, pos.z,
				look.x, look.y, look.z,
				up.x, up.y, up.z
			);
		}
	}

	void actualizar(Vec3 posAuto, float rot) {
		/*Actualiza la posicion y orientacion de la camara 
		segun la localizacion del jugador
		*/
		if (tipoCamara == 1) vistaPlanta();
		else {
			parentarLocalizacion(posAuto, rot);
			actualizar();
		}
	}
	void parentarLocalizacion(Vec3 posAuto, float rot) {
		/*
		Situa la camara detras el auto dado valores iniciales
		*/
		look = Vec3( posAuto.x, posAuto.y + offsetA, look.z = posAuto.z);
		pos = Vec3(posAuto.x-(offsetD)*cos(rot*PI/180),
				   posAuto.y + offsetA,
				   posAuto.z+(offsetD)*sin(rot*PI / 180));
		up = Vec3(0, 1, 0);
	}

	void vistaPlanta() {
		/*
		Vista especial para mostrar todo el mapa del juego
		(Solo para uso de testing)
		*/
		radio = 14;
		lejos = 100;
		float x = escenario.base.q4.x - escenario.base.q1.x;
		float y = abs(escenario.base.q3.z - escenario.base.q2.z)/2;
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

	jugador.cargarAutomovil();
	camara.actualizar(jugador.obtenerPosicion(), jugador.obtenerRotacion());
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Camara
	camara.actualizar(jugador.obtenerPosicion(), jugador.obtenerRotacion());

	//Escenario
	escenario.dibujarCuadricula();
	
	//Objetos
	jugador.dibujarAutomovil();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	camara.reescalar(w,h);
	glutPostRedisplay();
}

void fixedUpdate() {
	if (global.obtenerEstadoTecla('w')) jugador.acelerar(); 
	if (global.obtenerEstadoTecla('s')) jugador.frenar();
	if (global.obtenerEstadoTecla('r')) jugador.retroceder();
	if (global.obtenerEstadoTecla('a')) jugador.girar(1);
	if (global.obtenerEstadoTecla('d')) jugador.girar(-1);
	if (global.obtenerEstadoTecla(' ')) jugador.derrapar();
	jugador.actualizar();
	glutPostRedisplay();
}

void onTimer(int frame) {
	fixedUpdate();
	//jugador.imprimirStats();

	glutTimerFunc(frame, onTimer, frame);
}

void onKey(unsigned char tecla, int x, int y) {
	global.asignarTecla(tecla, true);
	if (tecla == '1' || tecla == '2') {
		camara.configurarTipoDeCamara(tecla - 48);
	}
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