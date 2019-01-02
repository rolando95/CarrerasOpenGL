#include "Juego.h"

Global::Global() {
	for (auto i = 0; i < ASCII; i++) {
		tecla[i] = false;
	}

	luzAmbiente.posicion = Vec4(1, 1, 0, 0);
	luzAmbiente.direccion = Vec3(0, -1, 0);
	luzAmbiente.ambiente = ambienteColor[1];
	luzAmbiente.asignarTipo(0);
	luzAmbiente.difuso = difusoColor[1];
	luzAmbiente.especular = Vec4(0, 0, 0, 1);

}

void Global::parentarPosFondo(Vec3 * posObj){
	free(parentPos);
	parentPos = posObj;
}

void Global::actualizarConfiguracionesGlobales(){

	if (modoSolido) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Poner fondo de hora de dia
		glClearColor(0, 0, 0.2, 1);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//Poner fondo blanco
		glClearColor(1, 1, 1, 1);
		glColor3f(0, 0, 0);
	}
}

void Global::asignarTecla(char pos, bool valor) {
	tecla[pos] = valor;
	if ((pos == 'p' || pos == 'P') && valor==1) pausa = !pausa;
	if ((pos == 'l' || pos == 'L') && valor == 1) {
		horario = (horario+1) % 2; //Alterna valores 0 y 1
		luzAmbiente.difuso = difusoColor[horario];
		luzAmbiente.ambiente = difusoColor[horario];
		materialFondo.emision = fondoEmisionColor[horario];
	}
	if ((pos == 'i' || pos == 'I') && valor == 1) {
		modoSolido = !modoSolido;

		if(modoSolido){
			//Iluminacion
			glEnable(GL_LIGHTING);

			//Texturas
			glEnable(GL_TEXTURE_2D);
		}
		else {
			//Iluminacion
			glDisable(GL_LIGHTING);

			//Texturas
			//glDisable(GL_TEXTURE_2D);
		}
	}
}

bool Global::obtenerEstadoTecla(char pos) {
	return tecla[pos];
}

int Global::obtenerHorario()
{
	return horario;
}

bool Global::obtenerPausa() { return pausa; }

bool *Global::obtenerPosTecla(char pos) {
	return &tecla[pos];
}

void Global::cargarFondo()
{
	//Imagen de fondo

	materialFondo.emision = fondoEmisionColor[1];
	materialFondo.difuso = Vec4(0, 0, 0, 1);
	materialFondo.ambiente = Vec4(0, 0, 0, 1);
	materialFondo.especular = Vec4(0, 0, 0, 1);

	texturaFondo[0].cargarTextura(urlFondoDia);
	texturaFondo[1].cargarTextura(urlFondoNoche);

	float nLados = 10;

	float apotema = distanciaDibujado;
	float lado = apotema * tan(PI / nLados);

	float alto = 500;

	GLfloat pts[4][3] = {
	{-lado, -alto, 0},
	{ lado, -alto, 0},
	{ lado,  alto, 0},
	{-lado,  alto, 0}
	};

	meshFondo = glGenLists(1);
	glNewList(meshFondo, GL_COMPILE);
	for (auto i = 0; i < nLados; i += 1) {
		glPushMatrix();
		glRotatef(-i * 360 / nLados, 0, 1, 0);
		glTranslatef(0, 0, -apotema);
		quadtex(pts[0], pts[1], pts[2], pts[3], i / nLados, (i + 1) / nLados, 0, 1, 1, 1);
		glPopMatrix();
	}
	glEndList();

	//Mar
	texturaOceano.cargarTextura(urlOceano);
}

void Global::dibujarFondo()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//Imagen de fondo
	materialFondo.actualizarGlMaterialfv();
	texturaFondo[horario].actualizar();
	glTranslatef(parentPos->x, parentPos->y, parentPos->z);
	glCallList(meshFondo);
	glPopMatrix();

	//Mar
	materialOceano.actualizarGlMaterialfv();
	texturaOceano.actualizar();
	quadtex(oceanoPts[0], oceanoPts[1], oceanoPts[2], oceanoPts[3], 0, 5, 0, 6, 1, 1);
	glPopAttrib();
}