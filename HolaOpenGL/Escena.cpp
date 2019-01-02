#include "Juego.h"

void Pista::cargarPista() {

	materialPista.difuso = Vec4(0.3, 0.3, 0.3, 1);
	texturaPista.cargarTextura(urlCarretera);

	materialTerreno.especular = Vec4(0, 0, 0, 0);
	texturaTerreno[0].cargarTextura(urlAcantilado2);
	texturaTerreno[1].cargarTextura(urlTerreno);
	texturaTerreno[2].cargarTextura(urlAcantilado);

	i = 0;
	ti = 0;
	float j;
	//Planta del pie
	agregarRecta(Vec3(3, 0, 0), Vec3(-4, 0, 0));

	//Punta del pie
	agregarCurva(Vec3(-4, 0, -2.5), 270, 90, 2.5);

	//Tobillo
	agregarCurva(Vec3(-4, 0, -6), -90, 60, 1);

	//Barriga
	agregarRecta(Vec3(-3.5, 0, -6.90), Vec3(-4.75, 0, -8.65));
	agregarCurva(Vec3(-6, 0, -7), 45, 90, 2);

	//Brazo
	agregarRecta(Vec3(-6, 0, -9), Vec3(-8, 0, -9));
	agregarCurva(Vec3(-8, 0, -10.5), 270, 90, 1.5);
	agregarCurva(Vec3(-8, 0, -13), -90, -5, 1);

	//Cuello
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
	agregarRecta(Vec3(0, 0, -13), Vec3(2, 0, -13));
	agregarCurva(Vec3(3, 0, -10), 90, 75, 3);
	agregarCurva(Vec3(6, 0, -15), -120, -60, 3);
	//agregarCurva(Vec3(9, 0, -10), 105, 90, 3);

	//Cola
	agregarCurva(Vec3(9, 0, -12), 115, -45, 1);
	agregarRecta(Vec3(8, 0, -9.5), Vec3(5, 0, -6.5));
	agregarCurva(Vec3(4.5, 0, -5.5), 135, 255, 0.5);

	//Talon
	agregarCurva(Vec3(4, 0, -2.5), 75, -90, 2.5);
	agregarRecta(Vec3(4, 0, 0), Vec3(3, 0, 0));
}

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
			//Pista
			puntos[i][0] = inf;
			puntos[i++][1] = sup;
			
			//El terreno tiene 1/resRect de la resolucion de la pista
			if (i % resRect == 0) {
				terreno[ti][0]   = Vec3(in.x + unidad.z*d * aC, in.y-altura, in.z - unidad.x*d * aC);
				terreno[ti][1]   = Vec3(in.x + unidad.z*d * aT, in.y-base, in.z - unidad.x*d * aT);
				terreno[ti][2]   = Vec3(in.x - unidad.z*d * aT, in.y-base, in.z + unidad.x*d * aT);
				terreno[ti++][3] = Vec3(in.x - unidad.z*d * aC, in.y+altura, in.z + unidad.x*d * aC);
			}
			
		}

		sup.x += unidad.x*cambio;
		sup.z += unidad.z*cambio;

		inf.x += unidad.x*cambio;
		inf.z += unidad.z*cambio;

		in.x += unidad.x*cambio;
		in.z += unidad.z*cambio;
	}
}

void Pista::agregarCurva(Vec3 centro, float anguloI, float anguloF, float radio, int iter, bool autoScale) {
	if (autoScale) {
		centro = { centro.x*s, centro.y*s, centro.z*s };
		radio *= s;
	}

	if (iter < 1) iter = abs(anguloF - anguloI) / defaultIter * 2 * ((radio / s < 2) ? 2 : radio / s);


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

		//Porcentaje de radio extra en las curvas (Para que el extremo del terreno no se pegue con la calle)
		static float extra = 1.05; 

		if (i%resCurva == 0) {
			if (n == 0) {
				//Parte Baja del terreno
				terreno[ti][0] = {
					(GLfloat)(centro.x + radio * cos(angulo) + d * cos(angulo)*aC*extra),
					(GLfloat)(centro.y - 10),
					(GLfloat)(centro.z - radio * sin(angulo) - d * sin(angulo)*aC*extra)
				};

				//Parte Media del terreno
				terreno[ti][1] = {
					(GLfloat)(centro.x + radio * cos(angulo) + d * cos(angulo)*aT*extra),
					(GLfloat)(centro.y - base),
					(GLfloat)(centro.z - radio * sin(angulo) - d * sin(angulo)*aT*extra)
				};
				terreno[ti][2] = {
					(GLfloat)(centro.x + radio * cos(angulo) - d * cos(angulo)*aT),
					(GLfloat)(centro.y - base),
					(GLfloat)(centro.z - radio * sin(angulo) + d * sin(angulo)*aT)
				};

				//Parte Alta del terreno
				terreno[ti++][3] = {
					(GLfloat)(centro.x + radio * cos(angulo) - d * cos(angulo)*aC),
					(GLfloat)(centro.y + altura),
					(GLfloat)(centro.z - radio * sin(angulo) + d * sin(angulo)*aC)
				};
			}

			else {
				//Parte Alta del terreno
				terreno[ti][3] = {
					(GLfloat)(centro.x + radio * cos(angulo) + d * cos(angulo)*aC*extra),
					(GLfloat)(centro.y + altura),
					(GLfloat)(centro.z - radio * sin(angulo) - d * sin(angulo)*aC*extra)
				};

				//Parte Media del terreno
				terreno[ti][2] = {
					(GLfloat)(centro.x + radio * cos(angulo) + d * cos(angulo)*aT*extra),
					(GLfloat)(centro.y - base),
					(GLfloat)(centro.z - radio * sin(angulo) - d * sin(angulo)*aT*extra)
				};
				terreno[ti][1] = {
					(GLfloat)(centro.x + radio * cos(angulo) - d * cos(angulo)*aT),
					(GLfloat)(centro.y - base),
					(GLfloat)(centro.z - radio * sin(angulo) + d * sin(angulo)*aT)
				};

				//Parte Baja del terreno
				terreno[ti++][0] = {
					(GLfloat)(centro.x + radio * cos(angulo) - d * cos(angulo)*aC),
					(GLfloat)(centro.y - altura),
					(GLfloat)(centro.z - radio * sin(angulo) + d * sin(angulo)*aC)
				};
			}
		}
		
		angulo += cambio;
	}
}

void Escenario::dibujarPista(bool forzarDibujarTodo) {
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	materialPista.actualizarGlMaterialfv();
	texturaPista.actualizar();
	float mod;

	float despIn = 0, despFi; //Inicio y fin del dibujo de las texturas
	static float texCoef = 0.0075;

	//Pista
	for (auto j = 0; j < i - 1; j++) {
		mod = modulo(puntos[j][0], puntos[j + 1][0]);
		despFi = despIn + mod * texCoef;
		if (modulo(puntos[j][0], *parentPos) < distanciaDibujado || forzarDibujarTodo) {
			if (mod < 1) mod = 1;
			quadtex(
				puntos[j][0].glVec3(),
				puntos[j][1].glVec3(),
				puntos[j + 1][1].glVec3(),
				puntos[j + 1][0].glVec3(),
				0, 1,
				despIn, despFi,
				15, (int)mod
			);
		}
		despIn = despFi - (int)despFi;
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

void Escenario::dibujarTerreno(bool forzarDibujarTodo)
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	float mod, modTex;

	materialTerreno.actualizarGlMaterialfv();
	//Terreno
	float distancia;
	static float coef = 0.25;

	float despIn=0, despFi=0; //Inicio y fin del dibujo de las texturas
	static float texCoef = 0.01;
	
	int numX, numY;

	for (auto j = 0; j < ti - 1; j++) {
		mod = modulo(terreno[j][3], *parentPos);
		modTex = modulo(terreno[j][3], terreno[j + 1][3]);
		despFi = despIn + modTex * texCoef;

		if(mod< distanciaDibujado || forzarDibujarTodo){
			if (mod < distanciaDibujado / 3) {
				numX = (modTex*coef<10)? 10 : modTex*coef;
			}else {
				numX = 2;
			}
			for (auto k = 0; k < 3; k++) {
				texturaTerreno[k].actualizar();
				quadtex(
					terreno[j + 1][k].glVec3(),
					terreno[j][k].glVec3(),
					terreno[j][k + 1].glVec3(),
					terreno[j + 1][k + 1].glVec3(),
					despFi, despIn,
					0, 1,
					numX,numX
				);
			}
		}
		despIn = despFi - (int)despFi;
	}
	
	if(tipo==1){
		mod = modulo(terreno[ti-1][3], *parentPos);
		if (mod < 1) mod = 1;
		for (auto k = 0; k < 3; k++) {
			texturaTerreno[k].actualizar();
			quadtex(
				terreno[0][k].glVec3(),
				terreno[ti - 1][k].glVec3(),
				terreno[ti - 1][k + 1].glVec3(),
				terreno[0][k + 1].glVec3(),
				despFi, despFi+1,
				0, 1,
				numX, numX
			);
		}
	}

	glPopAttrib();
	glPopMatrix();
}

void Escenario::parentarPosFondo(Vec3 * posObj)
{
	parentPos = posObj;
}

void Escenario::dibujarCuadricula() {
	//cuadricula base
	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glScalef(s, s, s);
	ejes();
	glPopMatrix();
	glPushAttrib(GL_FRONT_AND_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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