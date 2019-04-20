#include "Juego.h"
void Interfaz::ajustarProporcionPantalla()
{
	glScalef(parentResolucion->y / parentResolucion->x, 1, 1);
}
void Interfaz::cargarInterfaz()
{
	circuitoMapa.cargarTextura(urlCircuitoMapa);
	posAutoMapa.cargarTextura(urlAutomovilPosicionMapa);
	velocimetroInterior.cargarTextura(urlVelocimetroInterior);
	velocimetroExterior.cargarTextura(urlVelocimetroExterior);
	texto.cargarImagenes();
}

void Interfaz::parentarPosObjeto(Vec3 * posAuto, float * rot, float *velocidad, float maximaVelocidadReversa, float maximaVelocidad)
{
	parentPos = posAuto;
	parentRot = rot; //Rotacion de referencia del objeto al que registrar� en el mapa (automovil)
	parentVelocidad = velocidad;
	parentMaxVR = maximaVelocidadReversa;
	parentMaxV = maximaVelocidad;
}

void Interfaz::parentarResolucionVentana(Vec2 * resolucion)
{
	parentResolucion = resolucion;
}

void Interfaz::dibujarInterfaz()
{
	static float offsetX = 0.1; //Distancia relativa con respecto a la esquina de la ventana
	static float offsetY = 0.1; //
	
	
	//static float tam = 30;
	float z = 0.01;

	glDepthMask(GL_FALSE);
	pushAtributosObjetos();{
		glDisable(GL_LIGHTING);
		glColor3f(1, 1, 1);
		//Mapa
		glTranslatef(-1+offsetX, -1+offsetY, 0);
		ajustarProporcionPantalla();
		
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
		ajustarProporcionPantalla();

		glTranslatef(-.25, .25, 0);
		//Velocimetro interior y exterior
		glPushMatrix();
		{
			glScalef(0.25, 0.25, 0.25);
			
			//Exterior
			velocimetroExterior.actualizar();
			Plano2D();
			
			//Interior
			velocimetroInterior.actualizar();
			glTranslatef(0, -0.025, z);

			glRotatef(135, 0, 0, 1); //Alinear la manecilla con el inicio del velocimetro
			if (*parentVelocidad >= 0) {
				glRotatef(-*parentVelocidad / parentMaxV * 225, 0, 0, 1);
			}
			else{
				glRotatef(*parentVelocidad / parentMaxVR * 225, 0, 0, 1);
			}
			glScalef(0.9, 0.9, 1);
			Plano2D();
		}
		glPopMatrix();

		glTranslatef(0, 0, 2 * z);
		//Digitos
		glPushMatrix();
		{	
			glTranslatef(0.05, -0.20, 0);
			glScalef(0.15, 0.15, 1);
			texto.entero(*parentVelocidad * 3600 / 1000, 3);
		}
		glPopMatrix();
	}popAtributosObjetos();
	glDepthMask(GL_TRUE);
}

void Interfaz::dibujarPausa()
{
	glDepthMask(GL_FALSE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glPushMatrix(); {
		glColor4f(0, 0, 0, 0.5);
		Plano2D();
		ajustarProporcionPantalla();
		glColor3f(1, 1, 1);
		glScalef(0.1, 0.1, 0.1);
		texto.palabra("PAUSA", 0, 0);

	}glPopMatrix();
	glPopAttrib();
	glDepthMask(GL_TRUE);
}

void Interfaz::imprimirStats(bool inicio) {
	int i = 0;

	static int anterior = 0;

	int ahora = (int)time(0);
	static int conteo = 0;
	static int fps = 0;
	conteo += 1;
	if (ahora - anterior > 1) {
		fps = conteo;
		conteo = 0;
		anterior = ahora - 1;
	}

	cout << "Estado del Automovil" << endl; i++;
	cout << "______________________" << endl; i++;
	cout << "Pos x: " << round(parentPos->x * 100) / 100 << " m\t\t" << endl; i++;
	cout << "Pos z: " << round(parentPos->z * 100) / 100 << " m\t\t" << endl; i++;
	cout << "Velocidad: " << *parentVelocidad * 3600 / 1000 << " km/h \t\t" << endl; i++;
	cout << "Rotacion : " << *parentRot << " grados\t\t" << endl; i++;
	cout << "FPS: " << fps << "\t\t" << endl; i++;
	static CONSOLE_SCREEN_BUFFER_INFO coninfo;
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &coninfo);
	coninfo.dwCursorPosition.Y -= i;    // move up one line
	coninfo.dwCursorPosition.X = 0;    // move to the right the length of the word
	SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
}