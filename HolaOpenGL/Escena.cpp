#include "Juego.h"

void Pista::agregarRecta(Vec3 in, Vec3 fi, bool pushFirst, int iter) {
	in = { in.x*s, in.y*s, in.z*s };
	fi = { fi.x*s, fi.y*s, fi.z*s };

	if (iter < 1) iter = modulo(in, fi) / defaultIter;
	if (iter < 2) iter = 2;

	float cambio = modulo(in, fi) / iter;
	Vec3 unidad = normalizar(in, fi);
	Vec3 inf(
		in.x + unidad.z*d,
		in.y,
		in.z - unidad.x*d
	);

	Vec3 sup(
		in.x - unidad.z*d,
		in.y,
		in.z + unidad.x*d
	);

	for (auto j = 0; j < iter; j++) {
		if (pushFirst || j > 0) {
			puntos[i][0] = inf;
			puntos[i++][1] = sup;
		}

		sup.x += unidad.x*cambio;
		sup.z += unidad.z*cambio;

		inf.x += unidad.x*cambio;
		inf.z += unidad.z*cambio;
	}
}

void Pista::agregarCurva(Vec3 centro, float anguloI, float anguloF, float radio, int iter, bool autoScale) {
	if (autoScale) {
		centro = { centro.x*s, centro.y*s, centro.z*s };
		radio *= s;
	}

	if (iter < 1) iter = abs(anguloF - anguloI) / defaultIter * 2 * ((radio / s < 1) ? 0.5 : radio / s);


	float cambio = (anguloF - anguloI)*PI / 180 / iter;

	float angulo = anguloI * PI / 180;
	int n = 0;
	if (anguloF > anguloI) n = 1;

	for (auto j = 0; j < iter; j++) {
		puntos[i][n] = {
			centro.x + radio * cos(angulo) + d * cos(angulo),
			centro.y,
			centro.z - radio * sin(angulo) - d * sin(angulo),
		};
		puntos[i++][1 - n] = {
			centro.x + radio * cos(angulo) - d * cos(angulo),
			centro.y,
			centro.z - radio * sin(angulo) + d * sin(angulo),
		};
		angulo += cambio;
	}
}

void Pista::cargarYoshi() {

	materialPista.difuso = Vec4(0.3, 0.3, 0.3, 1);

	i = 0;
	float j;
	//Planta del pie
	agregarRecta(Vec3(4, 0, 0), Vec3(-4, 0, 0));

	//Punta del pie
	agregarCurva(Vec3(-4, 0, -2.5), 270, 90, 2.5);

	//Tobillo
	agregarCurva(Vec3(-4, 0, -6), -90, 60, 1);

	//Barriga
	//agregarRecta(Vec3(-4, 0, -7), Vec3(-5, 0, -8));
	agregarCurva(Vec3(-6, 0, -7), 60, 90, 2);

	//Brazo
	agregarRecta(Vec3(-6, 0, -9), Vec3(-8, 0, -9));
	agregarCurva(Vec3(-8, 0, -10.5), 270, 90, 1.5);
	agregarCurva(Vec3(-8, 0, -13), -90, -15, 1);

	//Cuello (adelante)
	agregarRecta(Vec3(-7, 0, -13), Vec3(-6.5, 0, -14.9));

	//Nariz
	agregarCurva(Vec3(-7.5, 0, -15), -15, 90, 1);
	agregarRecta(Vec3(-7.5, 0, -16), Vec3(-10, 0, -16));
	agregarCurva(Vec3(-10, 0, -19), 270, 90, 3);
	agregarRecta(Vec3(-10, 0, -22), Vec3(-9, 0, -22));
	agregarCurva(Vec3(-9, 0, -24), -90, 0, 2);

	//Ojos
	agregarCurva(Vec3(-4.5, 0, -24), 180, 0, 2.5);
	agregarCurva(Vec3(-1, 0, -24), 180, 270, 1);
	agregarCurva(Vec3(-1, 0, -27), 270, 285, 4);

	//Nuca
	agregarCurva(Vec3(1, 0, -22), 115, -45, sqrt(2));
	agregarCurva(Vec3(2.25, 0, -20.5), 135, 225, 0.5);
	agregarCurva(Vec3(1, 0, -19), 45, -45, sqrt(2));
	agregarCurva(Vec3(2.25, 0, -17.5), 135, 225, 0.5);
	agregarCurva(Vec3(1, 0, -16), 45, -45, sqrt(2));
	agregarRecta(Vec3(1.925, 0, -15), Vec3(-0.25, 0, -14));
	agregarCurva(Vec3(0, 0, -13.5), 135, 270, 0.5);

	//Espalda
	agregarRecta(Vec3(0, 0, -13), Vec3(3, 0, -13));
	agregarCurva(Vec3(3, 0, -10), 90, 75, 3);
	agregarCurva(Vec3(6, 0, -15), -120, -60, 3);
	agregarCurva(Vec3(9, 0, -10), 105, 90, 3);

	//Cola
	agregarCurva(Vec3(9, 0, -12), 90, -45, 1);
	agregarRecta(Vec3(7, 0, -9), Vec3(5, 0, -6));
	agregarCurva(Vec3(4.5, 0, -5.5), 135, 270, 0.5);

	//Talon
	agregarCurva(Vec3(4, 0, -2.5), 75, -75, 2.5);


}

void Pista::dibujarPista() {
	glPushMatrix();
	glPushAttrib(GL_FRONT_AND_BACK);
	materialPista.actualizarGlMaterialfv();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	float mod;
	for (auto j = 0; j < i - 1; j++) {
		mod = modulo(puntos[j][0], puntos[j + 1][0]);
		if (mod < 1) mod = 1;
		quad(
			puntos[j][0].glVec3(),
			puntos[j][1].glVec3(),
			puntos[j + 1][1].glVec3(),
			puntos[j + 1][0].glVec3(),
			15, (int)mod
		);
	}
	if (tipo == 1) {
		mod = modulo(puntos[i - 1][0], puntos[0][0]);
		if (mod < 1) mod = 1;
		quad(
			puntos[i - 1][0].glVec3(),
			puntos[i - 1][1].glVec3(),
			puntos[0][1].glVec3(),
			puntos[0][0].glVec3(),
			15, (int)mod
		);
	}
	glPopAttrib();
	glPopMatrix();
}

void Escenario::dibujarCuadricula() {
	//cuadricula base
	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glScalef(10, 10, 10);
	ejes();
	glPopMatrix();
	glPushAttrib(GL_FRONT_AND_BACK);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.9, 0.9, 0.9);

	//utilidades.h
	quad(base[0].glVec3(),
		base[1].glVec3(),
		base[2].glVec3(),
		base[3].glVec3(),
		25,
		30
	);

	glPopAttrib();
}