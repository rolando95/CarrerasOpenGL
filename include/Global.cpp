#include "Juego.h"
#pragma GCC diagnostic ignored "-Wchar-subscripts"
Global::Global() {
	for (auto i = 0; i < ASCII; i++) {
		tecla[i] = false;
		flipTecla[i] = false;
	}

	flipTecla[(int)interfaz] = true;

	luzAmbiente.posicion = Vec4(0, 1, 0, 0);
	luzAmbiente.direccion = Vec3(0, -1, 0);
	luzAmbiente.ambiente = ambienteColor[flipTecla[(int)horario]];
	luzAmbiente.difuso = difusoColor[flipTecla[horario]];
	luzAmbiente.especular = Vec4(0, 0, 0, 1);
	luzAmbiente.asignarTipo(0);

}

float Global::obtenerTiempo()
{
	return tiempo;
}

void Global::actualizar()
{
	float ahora = (float)glutGet(GLUT_ELAPSED_TIME)/1000;
	static float inicio = ahora;
	tiempo = ahora - inicio;
	
}

void Global::parentarResolucionVentana(Vec2 * resolucion)
{
	parentResolucion = resolucion;
}

void Global::cargarConfiguracionesGlobales() {
	glEnable(GL_DEPTH_TEST);

	//Iluminacion
	glEnable(GL_LIGHTING);

	//Texturas
	glEnable(GL_TEXTURE_2D);

	//Niebla
	glDisable(GL_FOG);


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);

	// Blending (Transparencias)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Global::actualizarConfiguracionesGlobales() {

	if (!flipTecla[(int)modoSolido]) {
		//Fijar color de borrado

		//Modo solido activado
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (flipTecla[horario]){
			glClearColor(0, 0.5, 0.8, 1);
			glFogfv(GL_FOG_COLOR, GRISCLARO);
		}else{
			glClearColor(0, 0.05, 0.1, 1);
			glFogfv(GL_FOG_COLOR, GRISOSCURO);
		}
		glFogf(GL_FOG_DENSITY, 0.01);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_FOG);
		//Poner fondo blanco
		glClearColor(1, 1, 1, 1);
		glColor3f(0, 0, 0);
	}
}

void Global::parentarPosFondo(Vec3 * posObj){
	parentPos = posObj;
}

void Global::asignarTecla(char pos, bool valor) {
	tecla[(int)pos] = valor;
	if (valor) {
		flipTecla[(int)pos] = !flipTecla[(int)pos];
	}

	if (pos >= 65 && pos <= 90) {
		pos += 32;
		tecla[(int)pos] = valor;
		if (valor) {
			flipTecla[(int)pos] = !flipTecla[(int)pos];
		}
	}

	if (pos == horario) {
		luzAmbiente.difuso = difusoColor[flipTecla[(int)horario]];
		luzAmbiente.ambiente = difusoColor[flipTecla[(int)horario]];
		materialFondo.emision = fondoEmisionColor[flipTecla[(int)horario]];
		luzAmbiente.asignarTipo(0);
	}

	if (pos == modoSolido) {
		if(!flipTecla[modoSolido]){
			//Iluminacion
			glEnable(GL_LIGHTING);

			//Texturas
			glEnable(GL_TEXTURE_2D);

			//Niebla
			if (flipTecla[niebla]) {
				glEnable(GL_FOG);
			}
		}
		else {
			//Iluminacion
			glDisable(GL_LIGHTING);

			//Texturas
			glDisable(GL_TEXTURE_2D);

			//Niebla
			glDisable(GL_FOG);
		}
	}

	if (pos == niebla) {
		if (flipTecla[niebla]) glEnable(GL_FOG);
		else glDisable(GL_FOG);
	}

	//Guardar captura
	if (pos == captura && valor) {
		//Hay un problema cuando se cambia el tama�o de la ventana ocasionalmente al hacer capturas
		static int resX = parentResolucion->x;
		static int resY = parentResolucion->y;

		sprintf(urlImagen, "%s%d.png", urlFichero, numCaptura % 100 + 100);
		numCaptura += 1;
		saveScreenshot(urlImagen, resX, resY);
	}
}

bool Global::obtenerEstadoTecla(char pos) { return tecla[pos]; }
bool Global::obtenerEstadoTeclaFlip(char pos) { return flipTecla[pos]; }

bool Global::obtenerHorario() { return flipTecla[horario]; }
bool Global::obtenerPausa() { return flipTecla[pausa]; }
bool Global::obtenerInterfaz() { return flipTecla[interfaz]; }

bool *Global::obtenerPosTecla(char pos) { return &tecla[pos]; }

//bool Global::obtenerCon
void Global::cargarFondo()
{
	//Imagen de fondo

	materialFondo.emision = fondoEmisionColor[flipTecla[horario]];
	materialFondo.difuso = Vec4(0, 0, 0, 1);
	materialFondo.ambiente = Vec4(0, 0, 0, 1);
	materialFondo.especular = Vec4(0, 0, 0, 1);

	texturaFondo[0].cargarTextura(urlFondoNoche);
	texturaFondo[1].cargarTextura(urlFondoDia);
	

	float nLados = 10;
	float apotema = distanciaDibujado;
	float lado = apotema * tan(PI / nLados);
	float alto = 500;

	meshFondo = glGenLists(1);
	glNewList(meshFondo, GL_COMPILE);
	for (auto i = 0; i < nLados; i += 1) {
		glPushMatrix();
		glRotatef(-i * 360 / nLados, 0, 1, 0);
		glTranslatef(0, 0, -apotema);
		quadtex(
			Vec3(-lado, -alto, 0),
			Vec3(lado, -alto, 0),
			Vec3(lado, alto, 0),
			Vec3(-lado, alto, 0),
			i / nLados, 
			(i + 1) / nLados,
			0, 1, 1, 1);
		glPopMatrix();
	}
	glEndList();

	//Mar
	texturaOceano.cargarTextura(urlOceano);
	materialOceano.difuso = Vec4(0.8, 0.8, 0.8, 0.3);
}

void Global::dibujarFondo()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//Imagen de fondo
	materialFondo.actualizar();
	texturaFondo[flipTecla[horario]].actualizar();
	glTranslatef(parentPos->x, parentPos->y - 10, parentPos->z);
	glCallList(meshFondo);
	glPopMatrix();
	glPopAttrib();
}

void Global::dibujarMar(bool detalleBajo)
{
	//Desplazamiento del mar con el paso del tiempo
	float desp = tiempo/200;
	glPushAttrib(GL_TEXTURE_BIT);
	//Mar
	materialOceano.actualizar();
	texturaOceano.actualizar();
	if (!detalleBajo)
		quadtex(oceanoPts[0], oceanoPts[1], oceanoPts[2], oceanoPts[3], 0+desp, 5 + desp, 0 + desp, 6 + desp, 100, 100);
	else{
		quadtex(oceanoPts[0]*2, oceanoPts[1]*2, oceanoPts[2]*2, oceanoPts[3]*2, 0 + desp, 5 + desp, 0 + desp, 6 + desp, 1, 1);
	}
	glPopAttrib();
}


void Global::imprimirControles() {
	cout << PROYECTO << endl;
	cout << "- Universidad Politecnica de Valencia -" << endl;
	cout << "\nControles" << endl;
	cout << "_________________________" << endl;
	cout << "Acelerar           -> [W]" << endl;
	cout << "Izquierda          -> [A]" << endl;
	cout << "Freno              -> [S]" << endl;
	cout << "Derecha            -> [D]" << endl;
	cout << "Retroceder         -> [R]" << endl;
	cout << "Freno de Mano      -> [Espacio]" << endl<<endl;
	cout << "Pausa              -> [P]" << endl;
	cout << "Camara helicoptero -> [1]" << endl;
	cout << "Camara 3ra persona -> [2]" << endl;
	cout << "Camara 1ra persona -> [3]" << endl << endl;
	cout << "Niebla on/off      -> [N]" << endl;
	cout << "Dia/Noche          -> [L]" << endl;
	cout << "Interfaz on/off    -> [C]" << endl;
	cout << "Modo Solido/Malla  -> [M]" << endl;
	cout << "Captura de pantalla-> [V] (Capturas/screenshotXXX.png)" << endl << endl << endl;
}