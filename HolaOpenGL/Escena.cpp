#include "Juego.h"

void Pista::cargarPista() {

	materialPista.difuso = Vec4(0.3, 0.3, 0.3, 1);
	texturaPista.cargarTextura(urlCarretera);

	materialTerreno.especular = Vec4(0, 0, 0, 0);
	texturaTerreno[0].cargarTextura(urlAcantilado2);
	texturaTerreno[1].cargarTextura(urlTerreno);
	texturaTerreno[2].cargarTextura(urlAcantilado);

	vistaHelicoptero.cargarTextura(urlVistaHelicoptero);

	i = 0;
	ti = 0;
	fi = 0;
	antFi = 0;

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

void Pista::agregarRecta(Vec3 in, Vec3 fin, bool pushFirst, int iter) {
	in = in*s;
	fin = fin*s;

	if (iter < 1) iter = modulo(fin, in) / defaultIter;
	if (iter < 2) iter = 2;

	float cambio = modulo(fin, in) / iter;
	Vec3 unidad = normalizar(fin, in);
	Vec3 sup(
		in.x + unidad.z*d,
		in.y,
		in.z - unidad.x*d
	);

	Vec3 inf(
		in.x - unidad.z*d,
		in.y,
		in.z + unidad.x*d
	);

	for (auto j = 0; j < iter; j++) {
		if (pushFirst || j > 0) {
			//Pista
			puntos[i][0] = sup;
			puntos[i++][1] = inf;
			
			//El terreno tiene 1/resRect de la resolucion de la pista
			if (i % resRect == 0) {
				terreno[ti][0]   = Vec3(in.x + unidad.z*d * aC, in.y-altura, in.z - unidad.x*d * aC);
				terreno[ti][1]   = Vec3(in.x + unidad.z*d * aT, in.y-base, in.z - unidad.x*d * aT);
				terreno[ti][2]   = Vec3(in.x - unidad.z*d * aT, in.y-base, in.z + unidad.x*d * aT);
				terreno[ti++][3] = Vec3(in.x - unidad.z*d * aC, in.y+altura, in.z + unidad.x*d * aC);
			}
			
		}

		sup = unidad * cambio + sup;
		inf = unidad * cambio + inf;
		in = unidad * cambio + in;


		//Farolas
		antFi += modulo(puntos[i - 1][1], puntos[max(0, i - 2)][1]);
		if (antFi > distFi) {
			antFi = 0;
			farolas[fi] = puntos[i - 1][1];
			angulofi[fi++] = anguloDeg(puntos[i - 1][0] - puntos[i - 1][1], (1, 0, 0)) * signoV(puntos[i - 1][0] - puntos[i - 1][1]).z * -1;
		}
	}
}

void Pista::agregarCurva(Vec3 centro, float anguloIn, float anguloFin, float radio, int iter, bool autoScale) {
	if (autoScale) {
		centro = centro * s;
		radio *= s;
	}

	if (iter < 1) iter = abs(anguloFin - anguloIn) / defaultIter * 2 * ((radio / s < 2) ? 2 : radio / s);


	float cambio = (anguloFin - anguloIn)*PI / 180 / iter;

	float angulo = anguloIn * PI / 180;
	int n = 0;
	if (anguloFin > anguloIn) n = 1;

	for (auto j = 0; j < iter; j++) {
		//sup
		puntos[i][n] = {
			centro.x + radio * cos(angulo) + d * cos(angulo),
			centro.y,
			centro.z - radio * sin(angulo) - d * sin(angulo),
		};
		//inf
		puntos[i++][1 - n] = {
			centro.x + radio * cos(angulo) - d * cos(angulo),
			centro.y,
			centro.z - radio * sin(angulo) + d * sin(angulo),
		};

		//Porcentaje de radio extra en las curvas (Para que el extremo del terreno no se pegue con la calle)
		static float extra = 1.05; 
		if (i%resCurva == 0) {
			if (n == 0) {
				//Parte Baja del terreno (sup)
				terreno[ti][0] = {
					centro.x + radio * cos(angulo) + d * cos(angulo)*aC*extra,
					centro.y - altura,
					centro.z - radio * sin(angulo) - d * sin(angulo)*aC*extra
				};

				//Parte Media del terreno
				terreno[ti][1] = {
					centro.x + radio * cos(angulo) + d * cos(angulo)*aT*extra,
					centro.y - base,
					centro.z - radio * sin(angulo) - d * sin(angulo)*aT*extra
				};
				terreno[ti][2] = {
					centro.x + radio * cos(angulo) - d * cos(angulo)*aT,
					centro.y - base,
					centro.z - radio * sin(angulo) + d * sin(angulo)*aT
				};

				//Parte Alta del terreno (inf)
				terreno[ti++][3] = {
					centro.x + radio * cos(angulo) - d * cos(angulo)*aC,
					centro.y + altura,
					centro.z - radio * sin(angulo) + d * sin(angulo)*aC
				};
			}

			else {
				//Parte Baja del terreno (sup)
				terreno[ti][0] = {
					centro.x + radio * cos(angulo) - d * cos(angulo)*aC,
					centro.y - altura,
					centro.z - radio * sin(angulo) + d * sin(angulo)*aC
				};

				//Parte Media del terreno
				terreno[ti][1] = {
					centro.x + radio * cos(angulo) - d * cos(angulo)*aT,
					centro.y - base,
					centro.z - radio * sin(angulo) + d * sin(angulo)*aT
				};
				terreno[ti][2] = {
					centro.x + radio * cos(angulo) + d * cos(angulo)*aT*extra,
					centro.y - base,
					centro.z - radio * sin(angulo) - d * sin(angulo)*aT*extra
				};

				//Parte Alta del terreno (inf)
				terreno[ti++][3] = {
					centro.x + radio * cos(angulo) + d * cos(angulo)*aC*extra,
					centro.y + altura,
					centro.z - radio * sin(angulo) - d * sin(angulo)*aC*extra
				};
			}
		}
		
		//Farolas
		antFi += modulo(puntos[i - 1][1], puntos[max(0,i - 2)][1]);
		if (antFi > distFi) {
			antFi = 0;
			farolas[fi] = puntos[i - 1][1];
			angulofi[fi++] = anguloDeg(puntos[i - 1][0] - puntos[i - 1][1], (1, 0, 0)) * signoV(puntos[i - 1][0] - puntos[i - 1][1]).z * -1;
		}
		angulo += cambio;
	}
}

void Escenario::dibujarPista(bool forzarDibujarTodo) {
	glPushMatrix();
	pushAtributosMateriales();

	materialPista.actualizar();
	texturaPista.actualizar();
	float mod;

	float despIn = 0, despFi; //Inicio y fin del dibujo de las texturas
	static float texCoef = 0.0075;

	//Pista
	for (auto j = 0; j < i - 1; j++) {
		mod = modulo(puntos[j + 1][0], puntos[j][0]);
		despFi = despIn + mod * texCoef;
		if (modulo(*parentPos, puntos[j][0]) < distanciaDibujado || forzarDibujarTodo) {
			if (mod < 1) mod = 1;
			quadtex(
				puntos[j][0],
				puntos[j][1],
				puntos[j + 1][1],
				puntos[j + 1][0],
				0, 1,
				despIn, despFi,
				15, (int)mod
			);
		}
		despIn = despFi - (int)despFi;
	}
	
	if (tipo == 1) {
		mod = modulo(puntos[0][0], puntos[i - 1][0]);
		if (mod < 1) mod = 1;
		quad(
			puntos[i - 1][0],
			puntos[i - 1][1],
			puntos[0][1],
			puntos[0][0],
			15, (int)mod
		);

	}
	popAtributosMateriales();
	glPopMatrix();
}

void Escenario::dibujarTerreno(bool detalleBajo, bool forzarDibujarTodo)
{
	pushAtributosObjetos(); {
		float mod, modTex;

		//Parte del terreno más cercana al automovil
		float cerca = modulo(*parentPos, terreno[0][3]);
		cercaFi = 0;

		materialTerreno.actualizar();

		//Terreno
		float distancia;
		static float coef = 0.25;

		float despIn = 0, despFi = 0; //Inicio y fin del dibujo de las texturas
		static float texCoef = 0.01;

		int numX, numY;

		for (auto j = 0; j < ti - 1; j++) {
			mod = modulo(*parentPos, terreno[j][3]);
			modTex = modulo(terreno[j + 1][3], terreno[j][3]);
			despFi = despIn + modTex * texCoef;

			if (mod < distanciaDibujado || forzarDibujarTodo) {
				if (mod < distanciaDibujadoHight && !detalleBajo) {
					numX = (modTex*coef < 10) ? 10 : modTex * coef;
				}
				else {
					numX = 2;
				}
				for (auto k = 0; k < 3; k++) {
					texturaTerreno[k].actualizar();
					quadtex(
						terreno[j + 1][k],
						terreno[j][k],
						terreno[j][k + 1],
						terreno[j + 1][k + 1],
						despFi, despIn,
						0, 1,
						numX, numX
					);
				}

				if (mod < cerca) {
					cerca = mod;
					cercaFi = j;
				}
			}
			despIn = despFi - (int)despFi;
		}

		if (tipo == 1) {
			mod = modulo(*parentPos, terreno[ti - 1][3]);
			if (mod < 1) mod = 1;
			for (auto k = 0; k < 3; k++) {
				texturaTerreno[k].actualizar();
				quadtex(
					terreno[0][k],
					terreno[ti - 1][k],
					terreno[ti - 1][k + 1],
					terreno[0][k + 1],
					despFi, despFi + 1,
					0, 1,
					numX, numX
				);
			}
		}
		
	}popAtributosObjetos();
}

void Escenario::dibujarAssets(bool forzarDibujarTodo)
{
	float mod;
	float cerca = modulo(*parentPos, farolas[0]);
	int posCerca = 0;
	int jC;
	pushAtributosObjetos(); {
		//Farolas
		materialFarola.actualizar();
		for (auto j = 0; j < fi; j++) {
			mod = modulo(*parentPos, farolas[j]);
			if(mod<distanciaDibujado){
				glPushMatrix();
				glTranslatef(farolas[j].x, farolas[j].y, farolas[j].z);
				glRotatef(angulofi[j] + 90, 0, 1, 0);
				glTranslatef(0, 0, -1);
				glCallList(meshFarola);
				glPopMatrix();	
				if (mod < cerca) {
					cerca = mod;
					posCerca = j;
				}
			}
		}
	}popAtributosObjetos();

	//LucesFarolas
	for (auto j = 0; j < numLamparasFarolas; j++) {
		glPushMatrix();
		jC = (posCerca + j) % fi;
		glTranslatef(farolas[jC].x, farolas[jC].y, farolas[jC].z);
		glRotatef(angulofi[jC] + 90, 0, 1, 0);
		glTranslatef(0, 10, 5);
		glTranslatef(0, 0, -1);
		//glutSolidCube(3);
		lamparasFarolas[j].actualizar();
		//prueba.actualizar();
		glPopMatrix();
	}

	//Meta
	glCallList(meshMeta);
}

void Escenario::dibujarTexturaVistaHelicoptero()
{
	pushAtributosObjetos();
	glDepthMask(GL_FALSE);
	materialTerreno.actualizar();
	vistaHelicoptero.actualizar();
	
	static float despX = 0.25;
	static float despZ = 1;
	quad(
		Vec3((-14+despX)* s, 20 , despZ * s),
		Vec3( (16+despX)* s, 20 , despZ * s),
		Vec3( (16+despX)* s, 20 , (-30 + despZ) * s),
		Vec3((-14+despX)* s, 20 , (-30 + despZ) * s),
		1,1
	);
	glDepthMask(GL_TRUE);
	popAtributosObjetos();
}

void Escenario::asignarEstadoFarolas(int estado)
{
	for (auto i = 0; i < numLamparasFarolas; i++)
		if(estado)lamparasFarolas[i].habilitar();
		else lamparasFarolas[i].inhabilitar();
}

void Escenario::cargarAssets()
{
	//Farola
	materialFarola.ambiente = Vec4(0.3, 0.3, 0.3, 1);
	materialFarola.difuso = Vec4(.1, .1, .1, 1);
	//materialFarola.especular = Vec4(1, 1, 1, 1);
	//materialFarola.brillo = Vec4(75, 75, 75, 1);
	
	meshFarola = glGenLists(1);
	glNewList(meshFarola, GL_COMPILE);
	pushAtributosObjetos(); {
		glPushMatrix();{
			glTranslatef(0, 9.5, 0);
			glRotatef(0, 0, 1, 0);
			glutSolidCylinder(0.25, 5, 6, 5);
			glTranslatef(0, 0, 5);
			glRotatef(90, 1, 0, 0);
			glutSolidCylinder(0.5, 0.5, 5, 1);
		}glPopMatrix();
		glRotatef(-90, 1, 0, 0);
		glutSolidCylinder(0.25,10,6,5);
	}popAtributosObjetos();
	glEndList();
	
	
	//Lamparas
	for (auto i = 0; i < numLamparasFarolas; i++) {
		lamparasFarolas[i].difuso = Vec4(0.1, 0.1, 0.1, 1);
		lamparasFarolas[i].ambiente = Vec4(1.5, 1.5, 1.5, 1);
		lamparasFarolas[i].posicion = Vec4(0, 0, 0, 1);
		lamparasFarolas[i].especular = Vec4(0, 0, 0, 1);
		lamparasFarolas[i].asignarTipo(1);
		lamparasFarolas[i].spot = true;
		lamparasFarolas[i].spotCutOff = 90;
		lamparasFarolas[i].spotExponent = 5;
		lamparasFarolas[i].direccion = Vec3(0, -1, 0);
	}

	//Meta
	materialMetaPilares.difuso = Vec4(0.1, 0.1, 0.1, 1);
	materialMetaPilares.ambiente = Vec4(0.3, 0.3, 0.3, 1);
	texturaMetaParteSuperior.cargarTextura(urlBanderaMeta);
	texturaBlank.cargarTextura(urlBlank);
	meshMeta = glGenLists(1);
	glNewList(meshMeta, GL_COMPILE);
	pushAtributosObjetos(); {
		texturaBlank.actualizar();
		glTranslatef(-s/2, 0, 0);
		
		//Pilares
		static float despX = 15;
		static float despZ = 10;
		static float despY = 5;
		for (auto n = -1; n < 2; n+=2) {
			pushAtributosObjetos();
			materialMetaPilares.actualizar();
			glTranslatef(-despX, despY*2, despZ*n);
			glScalef(1, 10, 1);
			glRotatef(90, 1, 0, 0);
			glutSolidCylinder(1, 1, 8, 8);
			popAtributosObjetos();
		}

		//Bandera de meta
		pushAtributosObjetos();
		glDisable(GL_CULL_FACE);
		materialMetaParteSuperior.actualizar();
		texturaMetaParteSuperior.actualizar();
		quadtex(
			
			Vec3(-despX, despY + 2, -despZ),
			Vec3(-despX, despY + 2, despZ),
			Vec3(-despX, despY + 5, despZ),
			Vec3(-despX, despY + 5, -despZ),
			/*
			Vec3(0, 1, 0),
			Vec3(0, 1, 10),
			Vec3(10, 1, 10),
			Vec3(10, 1, 0),*/
			0, 1, 0, 1
		);
		glEnable(GL_CULL_FACE);
		popAtributosObjetos();

		//Linea de meta
		glPushMatrix();
			glTranslatef(-despX, 0, 0);
			glScalef(1, 0.05, despZ*2);
			glutSolidCube(1);
		glPopMatrix();
	}popAtributosObjetos();
	glEndList();

}

void Escenario::parentarPosFondo(Vec3 * posObj)
{
	parentPos = posObj;
}

void Escenario::dibujarCuadricula() {
	//Ejes
	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glScalef(s, s, s);
	ejes();
	glPopMatrix();

	//Lineas de la cuadricula
	glPushAttrib(GL_FRONT_AND_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	quad(base[0],
		base[1],
		base[2],
		base[3],
		25,
		30
	);

	glPopAttrib();
}