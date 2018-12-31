#include "Juego.h"

Global::Global() {
	for (auto i = 0; i < ASCII; i++) {
		tecla[i] = false;
	}
}

void Global::asignarTecla(char pos, bool valor) {
	tecla[pos] = valor;
	if ((pos == 'p' || pos == 'P') && valor==1) pausa = !pausa;
}

bool Global::obtenerEstadoTecla(char pos) {
	return tecla[pos];
}

bool Global::obtenerPausa() { return pausa; }

bool *Global::obtenerPosTecla(char pos) {
	return &tecla[pos];
}