#include "Juego.h"

Global::Global() {
	for (auto i = 0; i < ASCII; i++) {
		tecla[i] = false;
	}
}

void Global::actualizarConfiguracionesGlobales()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
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

bool Global::obtenerPausa() { return pausa; }

bool *Global::obtenerPosTecla(char pos) {
	return &tecla[pos];
}