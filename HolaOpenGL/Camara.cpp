#include "Juego.h"

void Camara::configurarTipoDeCamara(int tipo) {
	if (tipo >= 1 && tipo <= 2) tipoCamara = tipo;
}

void Camara::asignarArriba(Vec3 lUp) {
	up = lUp;
}

void Camara::asignarDimensionesEscenario(float ancho, float largo)
{
	dimEscenario.x = ancho;
	dimEscenario.y = largo;
}

void Camara::reescalar(int lW, int lH) {
	w = lW; h = lH;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	actualizarPerspectiva();
	glutPostRedisplay();
}

void Camara::actualizarPerspectiva() {
	float ra = float(w) / float(h);
	gluPerspective(angulo, ra, 0.2, lejos);
}

Vec3 * Camara::obtenerRefPosicion()
{
	return &pos;
}

void Camara::actualizar() {
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

void Camara::parentarPosObjeto(Vec3 *posAuto, float *rot) {
	parentPos = posAuto;
	parentRot = rot; //Rotacion de referencia del objeto al que sigue (automovil)
}

void Camara::vistaPlanta() {
	pos.x = 0;
	pos.y = dimEscenario.y*1.65;
	pos.z = -dimEscenario.y/2;

	look.x = 0;
	look.y = 0;
	look.z = -dimEscenario.y / 2;
	up = Vec3(0, 0, -1);
	gluLookAt(pos.x, pos.y, pos.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z);
}