#include "Propiedades.h"

Material::Material(
	Vec4 Iambiente,
	Vec4 Idifuso,
	Vec4 Iespecular,
	Vec4 Ibrillo,
	Vec4 Iemision) {
	ambiente = Iambiente;
	difuso = Idifuso;
	especular = Iespecular;
	brillo = Ibrillo;
	emision = Iemision;
}

void Material::actualizarGlMaterialfv() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente.glVec4());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso.glVec4());
	glMaterialfv(GL_FRONT, GL_EMISSION, emision.glVec4());
	glMaterialfv(GL_FRONT, GL_SHININESS, brillo.glVec4());
	glMaterialfv(GL_FRONT, GL_SPECULAR, especular.glVec4());
}

Lampara::Lampara(
	Vec4 Iambiente,
	Vec4 Idifuso,
	Vec4 Iespecular,
	Vec4 Iposicion,
	Vec4 Idireccion) {
	direccion = Idireccion;
	posicion = Iposicion;
	ambiente = Iambiente;
	difuso = Idifuso;
	especular = Iespecular;

	tipo = &posicion.w;
	num = numLamparas;
	numLamparas++;
}

void Lampara::asignarTipo(int valor) {
		*tipo = valor;
}

void Lampara::actualizarGlLightfv() {
	glLightfv(GL_LIGHT0 + num, GL_AMBIENT, ambiente.glVec4());
	glLightfv(GL_LIGHT0 + num, GL_DIFFUSE, difuso.glVec4());
	glLightfv(GL_LIGHT0 + num, GL_SPECULAR, especular.glVec4());
	glLightfv(GL_LIGHT0 + num, GL_POSITION, posicion.glVec4());
	if (*tipo == 1) {
		if (spot) {
			glLightf(GL_LIGHT0 + num, GL_SPOT_CUTOFF, spotCutOff);
			glLightf(GL_LIGHT0 + num, GL_SPOT_EXPONENT, spotExponent);
			glLightfv(GL_LIGHT0 + num, GL_SPOT_DIRECTION, direccion.glVec3());
		}
		glLightfv(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, atenuacion.glVec3());
	}
}

void Lampara::habilitar() {
		glEnable(GL_LIGHT0 + num);
}

//Inicializa el conteo de lamparas
int Lampara::numLamparas = 0;