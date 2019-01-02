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

void Lampara::inhabilitar() {
	glDisable(GL_LIGHT0 + num);
}

//Inicializa el conteo de lamparas
int Lampara::numLamparas = 0;

void Textura::cargarTextura(const char * ruta)
{
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	loadImageFile((char *)ruta);
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

void Textura::asignarTipoUV(int tipo)
{
	uv = tipo;
}

Plano2D::Plano2D(float x0, float y0, float x1, float y1) {
	Plano2DTex(0, 0, 1, 1, x0, y0, x1, y1);
}


Plano2DTex::Plano2DTex(float xmin, float ymin, float xmax, float ymax, float x0, float y0, float x1, float y1)
{
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glTexCoord2f(xmin, ymin);
	glVertex3f(x0, y0, 0);
	glTexCoord2f(xmax, ymin);
	glVertex3f(x1, y0, 0);
	glTexCoord2f(xmax, ymax);
	glVertex3f(x1, y1, 0);
	glTexCoord2f(xmin, ymax);
	glVertex3f(x0, y1, 0);
	glEnd();
	glPopMatrix();
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

void Texto::cargarImagenes() {
	letras.cargarTextura(urlLetras);
	digitos.cargarTextura(urlDigitos);
}

void Texto::entero(int num, int numDigitos)
{
	char var[10]; //cadena que será enviada como parámetro para imprimir palabra.
	char extra0[10] = ""; //Cadena con los 0's extra
	if (numDigitos > 0) {
		int num2 = (num==0)?1:abs(num);
		int pos = 0;
		while(num2 < pow(10, numDigitos-1)) {
			extra0[pos++] = '0';
			num2 *= 10;
		}
		extra0[pos] = '\0'; //Fin de cadena
	}
	sprintf_s(var, "%s%d", extra0, abs(num));
	palabra(var);
}

void Texto::palabra(const char * palabra)
{
	bool tipo = -1; //0->char 1 ->digito -1->error
	int pos = 0;

	int len = strlen(palabra);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();


	for (auto i = 0; i < len; i++) {
		if (palabra[i] >= 65 && palabra[i] <= 90) {
			tipo = 0; pos = palabra[i] - 65;
		}
		else if (palabra[i] >= 97 && palabra[i] <= 122) {
			tipo = 0; pos = palabra[i] - 97;
		}
		else if (palabra[i] >= 48 && palabra[i] <= 57) {
			tipo = 1; pos = palabra[i] - 48;
		}
		else tipo = -1;

		glPushMatrix();
		glScalef(0.5, 1, 1);
		if (tipo == 0) {
			letras.actualizar();
			Plano2DTex((float)pos / 26, 0, (float)(pos + 1) / 26, 1, i, 0, i + 1, 1);
		}
		else if (tipo == 1) {
			digitos.actualizar();
			Plano2DTex((float)pos / 10, 0, (float)(pos + 1) / 10, 1, i, 0, i + 1, 1);
		}
		glPopMatrix();
	}
	glPopMatrix();
	glPopAttrib();
}
