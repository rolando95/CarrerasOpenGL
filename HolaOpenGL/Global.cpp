#include "Juego.h"

Global::Global() {
	for (auto i = 0; i < ASCII; i++) {
		tecla[i] = false;
	}
}

void Global::asignarTecla(char pos, bool valor) {
	tecla[pos] = valor;
}

bool Global::obtenerEstadoTecla(char pos) {
	return tecla[pos];
}

bool *Global::obtenerPosTecla(char pos) {
	return &tecla[pos];
}