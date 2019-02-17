/*
	ProyectoFinal

	Juego de carreras con mucho Derrape!!

	@author: Rolando A. Rosales J.
	@version: v1.0 7/enero/2019
	@require: freeglut, FreeImage

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
Interfaz interfaz;
Colisiones2D colision;

bool isDrawing = false;

void mostrarCarga(int porcentaje) {
	if (porcentaje < 100)
		cout << "-- Cargando "<<porcentaje<<"% -- \r";
	else{
		cout << "-- Presione enter para empezar -- \r";
		_getch();
		cout << "								  \r";
	}
}

void init() {
	mostrarCarga(1);
	//Configuraciones globales iniciales
	global.cargarConfiguracionesGlobales();
	global.luzAmbiente.habilitar();
	global.cargarFondo();
	global.parentarPosFondo(automovil.obtenerRefPosicion());
	global.parentarResolucionVentana(camara.obtenerRefResolucion());

	mostrarCarga(20);
	//Configuraciones iniciales del escenario 
	escenario.cargarPista();
	escenario.parentarPosFondo(automovil.obtenerRefPosicion());
	escenario.cargarAssets();
	escenario.asignarEstadoFarolas(true);

	mostrarCarga(60);
	//Configuraciones iniciales del automovil
	bool *acelerar = global.obtenerPosTecla('w');
	bool *retroceder = global.obtenerPosTecla('r');
	bool *freno = global.obtenerPosTecla('s');
	bool *frenoDeMano = global.obtenerPosTecla(' ');
	bool *izquierda = global.obtenerPosTecla('a');
	bool *derecha = global.obtenerPosTecla('d');
	automovil.posicionarYOrientar(Vec3(0,0.01,0), 180);
	automovil.parentarControles(acelerar, retroceder, freno, frenoDeMano, izquierda, derecha);
	automovil.cargarAutomovil();

	//Configuraciones iniciales de colision
	mostrarCarga(70);
	colision.asignarMeshColision(2);
	colision.obtenerPuntosTerreno(escenario.terreno, escenario.ti); //Deberia acceder a estas variables utilizando funciones en vez de ser publicas
	colision.parentarObjeto(automovil.obtenerRefPosicion(), automovil.obtenerRefRotacion(), automovil.obtenerRefVelocidad());
	colision.parentarExtraRotObjeto(automovil.obtenerRefRotacionExtra());
	mostrarCarga(80);
	//Configuraciones iniciales de la cámara
	camara.asignarDimensionesEscenario(abs(escenario.base[0].x - escenario.base[1].x), abs(escenario.base[2].z - escenario.base[1].z));
	camara.parentarPosObjeto(automovil.obtenerRefPosicion(), automovil.obtenerRefRotacion());
	camara.actualizar();

	mostrarCarga(90);
	//Configuraciones iniciales de la interfaz
	interfaz.cargarInterfaz();

	Vec2 rango = automovil.obtenerRangoVelocidadLineal();
	interfaz.parentarPosObjeto(
		automovil.obtenerRefPosicion(),
		automovil.obtenerRefRotacion(),
		automovil.obtenerRefVelocidad(),
		rango.x,
		rango.y
	);
	interfaz.parentarResolucionVentana(camara.obtenerRefResolucion());
}

void display() {
	isDrawing = true;
	static int tipoCamara;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Camara
	camara.actualizar();

	glPushMatrix();
	{
		tipoCamara = camara.obtenerTipoCamara();

		//Global
		if(!global.obtenerPausa())global.actualizar();
		global.luzAmbiente.actualizar();
		global.actualizarConfiguracionesGlobales();
		if (tipoCamara != 1) {
			global.dibujarFondo();
		}

		//Automovil
		if (tipoCamara != 3)automovil.dibujarAutomovil();

		//Material base
		baseMaterial.actualizar();

		//Escenario
		//escenario.dibujarCuadricula();
		
		if (tipoCamara == 1){
			escenario.dibujarAssets(true);
			escenario.dibujarPista(true);
			escenario.dibujarTerreno(true, true);
			
		}
		else{
			escenario.dibujarAssets();
			escenario.dibujarPista();
			escenario.dibujarTerreno();
		}
		//if (tipoCamara == 1) escenario.dibujarVistaHelicoptero();
		escenario.dibujarTexturaVistaHelicoptero();

		//Objetos translucidos
		glDepthMask(GL_FALSE);

		if(tipoCamara!=1)global.dibujarMar();
		else global.dibujarMar(true);

		glDepthMask(GL_TRUE);
	}glPopMatrix();


	//Interfaz
	//*_________________________________________
	// Objetos traslucidos que van pegados a la camara
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();		// Apilar la modelview
	glLoadIdentity();	// Camara en posicion de defecto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();		// Apilar la projection
	glLoadIdentity();	// Vista por defecto
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);

	if (global.obtenerInterfaz()) interfaz.dibujarInterfaz();
	if (global.obtenerPausa()) interfaz.dibujarPausa();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
	//*__________________________________________________________*/

	
	glutSwapBuffers();
	if(!global.obtenerPausa())interfaz.imprimirStats();
	isDrawing = false;
}

void reshape(int w, int h) {

	camara.reescalar(w,h);
}

void update() {
	if (!global.obtenerPausa()) {
		automovil.actualizar();
		colision.colisionar(escenario.cercaFi); //Deberia acceder a estas variables utilizando funciones en vez de ser publicas
	}
	if(!isDrawing){
		glutPostRedisplay();
	}
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

	//Configurar lamparas
	int horario = global.obtenerHorario();

	//Coche
	if (horario == 0) automovil.asignarEstadoLucesDelanteras(true);
	else automovil.asignarEstadoLucesDelanteras(false);

	//Farolas
	if(horario == 0 && camara.obtenerTipoCamara() != 1)escenario.asignarEstadoFarolas(true);
	else escenario.asignarEstadoFarolas(false);
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

	global.imprimirControles();

	//Inicializar
	glutInit(&argc, argv);

	//Inicializar FreeImage
	FreeImage_Initialise();
	
	//Buffers a usar
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//Posicionar la ventana de dibujo en la pantalla
	glutInitWindowPosition(0, 0);
	//Tamanio de la pantalla
	glutInitWindowSize((int)camara.resolucion.x,(int)camara.resolucion.y);

	//Crear la ventana
	glutCreateWindow(PROYECTO);
	glutHideWindow();
	init();

	mostrarCarga(100);

	glutShowWindow();
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