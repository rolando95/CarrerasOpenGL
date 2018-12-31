/*
	ProyectoFinal

	Juego de carreras con mucho drift!!

	@author: Rolando A. Rosales J.
	@version: v1.0 x/dic/2018
	@require: freeglut

*/

#include <iostream>
#include "gl/freeglut.h"
#include "Juego.h"
#include <math.h>

Material baseMaterial;
Escenario escenario;
Global global;
Automovil automovil;
Camara camara;


double alpha=0;

void init() {
	//Fijar color de borrado
	glClearColor(0,0,0.2,1);
	
	glEnable(GL_DEPTH_TEST);

	//Iluminacion
	glEnable(GL_LIGHTING);

	//Texturas
	glEnable(GL_TEXTURE_2D);

	global.luzAmbiente.posicion = Vec4(1, 1, 0,0);
	global.luzAmbiente.direccion = Vec3(0, -1, 0);
	global.luzAmbiente.ambiente = Vec4(0.025,0.025,0.025, 1);
	global.luzAmbiente.asignarTipo(0);
	global.luzAmbiente.difuso = Vec4(.2, .2, .2, 1);
	global.luzAmbiente.especular = Vec4(0, 0, 0, 1);
	global.luzAmbiente.habilitar();

	escenario.cargarYoshi();
	escenario.cargarFondo();
	escenario.parentarPosFondo(automovil.obtenerRefPosicion());

	bool *acelerar = global.obtenerPosTecla('w');
	bool *retroceder = global.obtenerPosTecla('r');
	bool *freno = global.obtenerPosTecla('s');
	bool *frenoDeMano = global.obtenerPosTecla(' ');
	bool *izquierda = global.obtenerPosTecla('a');
	bool *derecha = global.obtenerPosTecla('d');
	automovil.parentarControles(acelerar, retroceder, freno, frenoDeMano, izquierda, derecha);
	automovil.cargarAutomovil();
	camara.obtenerDimensionesEscenario(abs(escenario.base[0].x - escenario.base[1].x), abs(escenario.base[2].z - escenario.base[1].z));
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

		global.luzAmbiente.actualizarGlLightfv();
		
		//Material base
		baseMaterial.actualizarGlMaterialfv();

		//Escenario
		//escenario.dibujarCuadricula();
		escenario.dibujarPista();
		escenario.dibujarTerreno();
		escenario.dibujarFondo();
		//Objetos
		if (!global.obtenerPausa())automovil.actualizar();
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



int main(int argc, char **argv) {
	automovil.imprimirControles();

	//Inicializar
	glutInit(&argc, argv);

	//Inicializar FreeImage
	FreeImage_Initialise();
	
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

	//Liberar FreeImage
	FreeImage_DeInitialise();

	return 0;
}