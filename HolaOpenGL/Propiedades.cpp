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

void Textura::cargarTextura(const char * ruta)
{
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	loadImageFile((char *)ruta);
}

void Textura::asignarTipoUV(int tipo)
{
	uv = tipo;
}

void Textura::actualizar()
{
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	if (uv == 2) {
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	}
}

void conecta4(int *matriz, int N, int M, int x, int y, int viejo, int  nuevo)
{
	if (x >= 0 && y >= 0 && x < N && y < M) {
		if (matriz[x*M+y] == viejo) {
			matriz[x*M+y] = nuevo;
			conecta4(matriz, N, M, x, y - 1, viejo, nuevo);
			conecta4(matriz, N, M, x, y + 1, viejo, nuevo);
			conecta4(matriz, N, M, x - 1, y, viejo, nuevo);
			conecta4(matriz, N, M, x + 1, y, viejo, nuevo);
		}
	}
}
