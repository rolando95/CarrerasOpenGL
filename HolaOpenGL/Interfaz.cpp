#include "Juego.h"
void Interfaz::cargarInterfaz()
{
	circuitoMapa.cargarTextura(urlCircuitoMapa);
	posAutoMapa.cargarTextura(urlAutomovilPosicionMapa);
	texto.cargarImagenes();
}

void Interfaz::parentarPosObjeto(Vec3 * posAuto, float * rot, float *velocidad)
{
	parentPos = posAuto;
	parentRot = rot; //Rotacion de referencia del objeto al que registrará en el mapa (automovil)
	parentVelocidad = velocidad;
}

void Interfaz::parentarResolucionVentana(Vec2 * resolucion)
{
	parentResolucion = resolucion;
}

void Interfaz::dibujarInterfaz()
{
	static float escala = 0.5;

	//static float altura = 0.5; //Alto del mapa en pantalla en base del viewport
	static float offsetX = 0.1; //Distancia relativa con respecto a la esquina de la ventana
	static float offsetY = 0.1; //
	float proporcion = parentResolucion->x / parentResolucion->y;
	
	static float tam = 30;

	//static Vec2 esc = {25, 30};
	//static Vec2 origen = { 14,0 };
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);

	float z = 0.01;
	//Mapa
	glPushMatrix();
	{
		glTranslatef(-1+offsetX, -1+offsetY, 0);
		glScalef(1 / proporcion, 1, 1);
		
		//Escala del mapa
		glScalef(0.5, 0.5, 0.5);

		//Circuito
		circuitoMapa.actualizar();
		Plano2D(0, 0, 1, 1);

		//Auto
		glTranslatef(0, 0, z); //Z

		float x = (parentPos->x / s + 14)/30;
		float y = (-parentPos->z / s + 1)/30;
		glTranslatef(x, y, 0);

		glScalef(0.05, 0.05, 1);
		glRotatef(*parentRot, 0, 0, 1);

		posAutoMapa.actualizar();
		Plano2D();
	}
	glPopMatrix();

	//Velocimetro
	glPushMatrix();
	{
		glTranslatef(1 - offsetX, -1 + offsetY, 0);
		glScalef(1 / proporcion, 1, 1);

		glScalef(0.15, 0.15, 1);
		glTranslatef(-1, 0, 0);
		texto.entero(*parentVelocidad* 3600 / 1000,3);
	}
	glPopMatrix();

	glPopAttrib();
}
