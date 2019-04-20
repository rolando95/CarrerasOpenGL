#include "Juego.h"

void Camara::configurarTipoDeCamara(int tipo) {
	if (tipo >= 1 && tipo <= 3) tipoCamara = tipo;
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
	resolucion.x = lW; resolucion.y = lH;
	glViewport(0, 0, (GLsizei)lW, (GLsizei)lH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	actualizarPerspectiva();
	glutPostRedisplay();
}

int Camara::obtenerTipoCamara()
{
	return tipoCamara;
}

void Camara::actualizarPerspectiva() {
	float ra = float(resolucion.x) / float(resolucion.y);
	gluPerspective(angulo, ra, 0.2, lejos);
}

Vec3 * Camara::obtenerRefPosicion()
{
	return &pos;
}

Vec2 * Camara::obtenerRefResolucion()
{
	return &resolucion;
}

void Camara::actualizar() {
	/*Actualiza la posicion y orientacion de la camara segun
	su posicion actual
	*/
	if (tipoCamara == 1) vistaPlanta();
	else{


		//Calculo de la nueva posicion de camara
		if(tipoCamara == 2){
			/*
			En 3ra persona Se posiciona la camara antes de hacer
			de hacer el calculo del frame actual.
			Crea un ligero retraso que mejorar la jugabilidad
			*/
			gluLookAt(pos.x, pos.y, pos.z,
				look.x, look.y, look.z,
				up.x, up.y, up.z
			);

			pos.x = parentPos->x - offsetD * cos(*parentRot*PI / 180);
			pos.y = parentPos->y + offsetA;
			pos.z = parentPos->z + offsetD * sin(*parentRot*PI / 180);

			look.x = parentPos->x;
			look.y = parentPos->y + offsetA - tan(offsetAngulo * PI / 180)*offsetA;
			look.z = parentPos->z;
		}
		else {
			static float dist = 0.01;
			pos.x = parentPos->x - dist * cos((*parentRot - *parentExtraRot)*PI / 180);
			pos.y = parentPos->y + offsetAFPS;
			pos.z = parentPos->z + dist * sin((*parentRot - *parentExtraRot)*PI / 180);

			look.x = parentPos->x;
			look.y = parentPos->y + offsetAFPS;// +offsetA - tan(offsetAngulo * PI / 180)*offsetA;
			look.z = parentPos->z;

			up = Vec3(0, 1, 0);
			gluLookAt(pos.x, pos.y, pos.z,
				look.x, look.y, look.z,
				up.x, up.y, up.z
			);

		}

		up = Vec3(0, 1, 0);
	}
}

void Camara::parentAutomovil(Automovil * obj)
{
	automovil = obj;
}

void Camara::parentarPosObjeto(Vec3 *posAuto, float *rot) {
	parentPos = posAuto;
	parentRot = rot; //Rotacion de referencia del objeto al que sigue (automovil)
}

void Camara::parentarExtraRotObjeto(float * extraRot)
{
	parentExtraRot = extraRot;
}

void Camara::vistaPlanta() {
	static int despZ = 1;
	pos.x = 0;
	pos.y = dimEscenario.y*1.65;
	pos.z = -dimEscenario.y/2 + despZ*s;

	look.x = 0;
	look.y = 0;
	look.z = -dimEscenario.y / 2 + despZ * s;
	up = Vec3(0, 0, -1);
	gluLookAt(pos.x, pos.y, pos.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z);
}