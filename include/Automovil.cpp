#include "Juego.h"

void Automovil::cargarAutomovil() {
	pintura.difuso = Vec4(1, 0, 0, 1);
	pintura.ambiente = Vec4(1.25, 0.1, 0.1, 1);
	pintura.especular = Vec4(1, 1, 1, 1);
	pintura.brillo = Vec4(75, 75, 75, 1);
	pintura.emision = Vec4(0, 0, 0, 1);

	mateGris.difuso = Vec4(0.3, 0.3, 0.3, 1);

	//mateNegro = mateGris;
	mateNegro.difuso = Vec4(0.05, 0.05, 0.05, 1);
	mateNegro.ambiente = Vec4(0.2,0.2, 0.2, 1);

	reflejo.cargarTextura(urlReflejoFondoNoche);
	reflejo.asignarTipoUV(2);

	mesh = glGenLists(1);
	glNewList(mesh, GL_COMPILE);
	pushAtributosObjetos();{
		//Ventana
		pushAtributosObjetos();{
			reflejo.actualizar();
			mateGris.actualizar();
			
			//Ventana trasera
			quad(
				Vec3(-1.8, 1.3, -0.85),
				Vec3(-1.8, 1.3, 0.85),
				Vec3(-1.35, 1.9, 0.6),
				Vec3(-1.35, 1.9, -0.6),
			2,2);

			//Ventanas laterales
			quad(
				Vec3(-1.70, 1.3, 0.9),
				Vec3( 0.90, 1.3, 0.9),
				Vec3( 0.50, 1.9, 0.6),
				Vec3(-1.15, 1.9, 0.6),
			2, 2);

			quad(
				Vec3(-1.15, 1.9, -0.6),
				Vec3(0.50, 1.9, -0.6),
				Vec3(0.90, 1.3, -0.9),
				Vec3(-1.70, 1.3, -0.9),
				2, 2);
		}popAtributosObjetos();
		

		//Mate Gris
		mateGris.actualizar();
		glPushMatrix();
		{
			//Base mitad (gris)
			glTranslatef(0, 0.65, 0);
			glScalef(5.4, 0.4, 2);
			glutSolidCube(1);
		}
		glPopMatrix();

		//Mate negro
		mateNegro.actualizar();
		glPushMatrix();
		{
			//Parte superior
			glPushMatrix();
			glTranslatef(-0.5, 1.9, 0);
			glScalef(1.7, 0.1, 1.3);
			glutSolidCube(1);
			glPopMatrix();

			//Placa del automovil
			glPushMatrix();
			glTranslatef(-2.696, 0.55, 0);
			glScalef(0.01, 0.3, 0.75);
			glutSolidCube(1);
			glPopMatrix();

			//Base inferior
			glTranslatef(0, 0.4, 0);
			glScalef(5, 0.1, 2);
			glutSolidCube(1);
		}
		glPopMatrix();

		//Pintura
		pushAtributosObjetos(); {
			pintura.actualizar();
			reflejo.actualizar();

			//Parte alta (color auto)
			for(auto n=-1; n<2; n+=2){
				//Pilares delanteros
				glPushMatrix();
				glTranslatef(1, 1.25, 0.85*n);
				glRotatef(-25*n, 0, 1, 0);
				glRotatef(50, 0, 0, 1);
				glRotatef(45, 0, 1, 0);
				glTranslatef(0, 0.4, 0);
				glScalef(0.2, 1, 0.2);
				glutSolidCube(1);
				glPopMatrix();

				//Traseros
				glPushMatrix();
				glTranslatef(-1.75, 1.25, 0.85*n);
				glRotatef(30*n, 0, 1, 0);
				glRotatef(-45, 0, 0, 1);
				glRotatef(45, 0, 1, 0);
				glTranslatef(0, 0.4, 0);
				glScalef(0.2, 0.8, 0.2);
				glutSolidCube(1);
				glPopMatrix();
			}

			//Base mitad (color auto)
			glTranslatef(0, 1.1, 0);
			glScalef(5, 0.5, 2);
			glutSolidCube(1);
		}popAtributosObjetos();

	}popAtributosObjetos();

	glEndList();

	//Luces traseras
	lucesTraserasMaterial.difuso = Vec4(0.1, 0, 0, 1);
	lucesTraserasMaterial.especular = Vec4(.5, .25, .25, 1);
	lucesTraserasMaterial.ambiente = Vec4(0.5, 0.3, 0.3, 1);
	
	lucesTraseras = glGenLists(1);
	glNewList(lucesTraseras, GL_COMPILE);
	for(auto n=0; n<2; n++){
		glPushMatrix();
		glTranslatef(-2.5, 1.1, 0.5-n);
		glScalef(0.1, 0.2, 0.35);
		glutSolidCube(1);
		glPopMatrix();
	}
	glEndList();

	lucesDelanteras.difuso = Vec4(10, 10, 10, 1);
	lucesDelanteras.ambiente = Vec4(0.8, 0.8, 0.8, 1);
	lucesDelanteras.posicion = Vec4(3, 1, 0, 1);
	lucesDelanteras.especular = Vec4(1, 1, 1, 1);
	lucesDelanteras.asignarTipo(1);
	lucesDelanteras.spot = true;
	lucesDelanteras.direccion = Vec3(1, 0, 0);
	lucesDelanteras.habilitar();
}

void Automovil::posicionarYOrientar(Vec3 lPos, float r) {
	pos = lPos; rot = r;
}

void Automovil::parentarControles(bool *acelerar, bool *retroceder, bool *freno, bool *frenoDeMano, bool *izquierda, bool *derecha) {
	parentAcelerar = acelerar;
	parentFreno = freno;
	parentFrenoDeMano = frenoDeMano;
	parentGiroIzquierda = izquierda;
	parentGiroDerecha = derecha;
	parentRetroceder = retroceder;
}

Vec3 *Automovil::obtenerRefPosicion() {
	return &pos;
}

Vec2 Automovil::obtenerRangoVelocidadLineal()
{
	return Vec2(vLMaxR, vLMax);
}

float *Automovil::obtenerRefRotacion() {
	return &rot;
}

float * Automovil::obtenerRefRotacionExtra()
{
	return &rotD;
}

float * Automovil::obtenerRefVelocidad()
{
	return &vL;
}

void Automovil::girar(float direccion) {
	if (vL != 0) {
		//En velocidad negativa se invierte la transmision 
		//del giro acorde a los autos reales
		if (vL < 0) direccion *= -1;
		float val = 1 / cosh((abs(vL) - cambio)*PI / 180);
		rot += direccion * vRot / FPS / 2 * val;
		
		//rot %= 360; //si rot fuese tipo entero...
		if (rot > 360) rot -= 360;
		else if (rot < 0) rot += 360;
	}
}

void Automovil::acelerar(float valor){
	//abs(valor);
	vL += valor / FPS;
;
	if (vL > vLMax) vL = vLMax;
}

void Automovil::retroceder(float valor) {
	//abs(valor);
	vL -= valor / FPS ;
	if (vL < -vLMaxR) vL = -vLMaxR;
}

void Automovil::desacelerar(float valor) {
	//abs(valor);
	if (vL > 0) {
		vL -= valor / FPS ;
		if (vL < 0) vL = 0;
	}
	else if (vL < 0) {
		vL += valor / FPS ;
		if (vL > 0) vL = 0;
	}
}

void Automovil::derrapar(float valor) {
	if (abs(vL) > vMinD)
		rotD += valor / FPS;
	else
		rotD += valor / FPS * abs(vL) / vMinD;
	if (rotD > rotDMax) rotD = rotDMax;
	else if (rotD < -rotDMax) rotD = -rotDMax;

}

void Automovil::amortiguarDerrape(float valor) {
	if (rotD != 0) {
		float reduccion = 0;
		if (rotD > 0) {
			rotD -= valor / FPS;
			if (rotD < 0) {
				reduccion = valor / FPS - abs(rotD);
				rotD = 0;
			}
			else reduccion = valor / FPS;
		}
		else if (rotD < 0) {
			rotD += valor / FPS;
			if (rotD > 0) {
				reduccion = -valor / FPS + rotD;
				rotD = 0;
			}
			else reduccion = -valor / FPS;
		}

		//El valor de rotacion extra reducido se transmite a rot
		rot += reduccion / 2;
	}
}

void Automovil::actualizar() {

	//Velocidad Lineal

	//Freno
	if (*parentFrenoDeMano) {
		if (vL > cambio+0.1) desacelerar(aL2*coefDrift);
		else desacelerar(aL1*coefDrift);
	}

	if (*parentFreno) desacelerar(aFL);
	//Acelerar
	else if (*parentAcelerar) {
		if (vL > cambio) acelerar(aL2);
		else acelerar(aL1);
		//if (*parentFrenoDeMano) desacelerar(aFDL/5);
	}
	

	else if (*parentRetroceder && !*parentFrenoDeMano) retroceder(aL2);
	else desacelerar(aNL);

	//Velocidad Angular
	if (!*parentGiroIzquierda && *parentGiroDerecha) {
		if (*parentFrenoDeMano) derrapar(-vR);
		girar(-vRot);
	}
	if (*parentGiroIzquierda && !*parentGiroDerecha) {
		if (*parentFrenoDeMano) derrapar(vR);
		girar(vRot);
	}

	if (!*parentFrenoDeMano) amortiguarDerrape(vAR);
	else amortiguarDerrape(vAR / 25);

	//Actualizacion de posicion
	pos.x += vL * escalaVelocidadL * cos((rot - rotD / 2)*PI / 180) / FPS;
	pos.z -= vL * escalaVelocidadL * sin((rot - rotD / 2)*PI / 180) / FPS;
}

void Automovil::dibujarAutomovil() {
	glPushMatrix();{

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(rot + rotD, 0, 1, 0);

		lucesDelanteras.actualizar();

		glCallList(mesh);


		//Luces traseras y ruedas
		glPushMatrix(); {
			if (*parentFrenoDeMano || *parentFreno || *parentRetroceder) {
				lucesTraserasMaterial.emision = Vec4(1, 0, 0, 1);
			}
			else {
				lucesTraserasMaterial.emision = Vec4(0, 0, 0, 1);
			}
			lucesTraserasMaterial.actualizar();
			glCallList(lucesTraseras);

			//Ruedas
			static float rot = 0;
			glPushMatrix();
			{
				for (auto x = -1; x < 2; x += 2) {
					for (auto z = -1; z < 2; z += 2) {
						glPushMatrix();
						glTranslatef(1.5*x, 0, z);
						glTranslatef(0, 0.5, 0);
						glScalef(0.5, 0.5, 0.5);
						glTranslatef(0, 0, -0.5);
						glRotatef(-rot * PI, 0, 0, 1);
						mateNegro.actualizar();
						glutSolidCylinder(1, 1, 10, 2);
						glPopMatrix();
					}
				}
				rot += vL;
				if (rot > 360) rot -= 360;
				else if (rot < 0) rot += 360;

			}glPopMatrix();
		}glPopMatrix();
	}glPopMatrix();
}

void Automovil::asignarEstadoLucesDelanteras(bool valor)
{
	if (valor) lucesDelanteras.habilitar();
	else lucesDelanteras.inhabilitar();
}
