#include "Juego.h"

void Automovil::cargarAutomovil() {
	pintura.difuso = Vec4(1, 0, 0, 1);
	pintura.ambiente = Vec4(1.25, 0.1, 0.1, 1);
	pintura.especular = Vec4(1, 1, 1, 1);
	pintura.brillo = Vec4(75, 75, 75, 1);
	
	reflejo.cargarTextura(urlReflejoFondoNoche);
	reflejo.asignarTipoUV(2);

	mesh = glGenLists(1);
	glNewList(mesh, GL_COMPILE);
	glPushMatrix();
	glScalef(5, 1.75, 2);
	glTranslatef(0, 0.5, 0);
	glutSolidCube(1);
	glPopMatrix();
	glEndList();

	lucesTraserasMaterial.difuso = Vec4(0.1, 0, 0, 1);
	lucesTraserasMaterial.especular = Vec4(.5, .25, .25, 1);
	lucesTraseras = glGenLists(1);

	glNewList(lucesTraseras, GL_COMPILE);
	glPushMatrix();
	glTranslatef(-2.5, 0.85, -0.5);
	glScalef(0.1, 0.18, 0.35);
	glutSolidCube(1);
	glPopMatrix();
	glEndList();

	lucesDelanteras.difuso = Vec4(25, 25, 25, 1);
	lucesDelanteras.ambiente = Vec4(1, 1, 1, 1);
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

float *Automovil::obtenerRefRotacion() {
	return &rot;
}

void Automovil::girar(float direccion) {
	if (vL != 0) {
		//En velocidad negativa se invierte la transmision 
		//del giro acorde a los autos reales
		if (vL < 0) direccion *= -1;
		float val = 1 / cosh((abs(vL) - cambio)*PI / 180);
		rot += direccion * vRot / FPS / 2 * val;
		if (rot > 360) rot -= 360;
		else if (rot < 0) rot += 360;
	}
}

void Automovil::acelerar(float valor){
	//abs(valor);
	vL += valor / FPS;
	if (vL > vLMax) vL = vLMax;
}

void Automovil::retroceder(float valor) {
	//abs(valor);
	vL -= valor / FPS;
	if (vL < -vLMaxR) vL = -vLMaxR;
}

void Automovil::desacelerar(float valor) {
	//abs(valor);
	if (vL > 0) {
		vL -= valor / FPS;
		if (vL < 0) vL = 0;
	}
	else if (vL < 0) {
		vL += valor / FPS;
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

		//El valor de rotacion extra reducido se
		//transmite a rot
		rot += reduccion / 2;
	}
}

void Automovil::actualizar() {

	//Velocidad Lineal
	if (*parentFreno) desacelerar(aFL);
	else if (*parentAcelerar) {
		if (vL > cambio)acelerar(aL2);
		else acelerar(aL1);
		if (*parentFrenoDeMano) desacelerar(aNL);
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
	pos.x += vL * cos((rot - rotD / 2)*PI / 180) / FPS;
	pos.z -= vL * sin((rot - rotD / 2)*PI / 180) / FPS;
}

void Automovil::imprimirControles() {
	cout << "\nControles" << endl;
	cout << "______________________\n" << endl;
	cout << "Acelerar      -> [W]" << endl;
	cout << "Izquierda     -> [A]" << endl;
	cout << "Freno         -> [S]" << endl;
	cout << "Derecha       -> [D]" << endl;
	cout << "Retroceder    -> [R]" << endl;
	cout << "Freno de Mano -> [Espacio]\n\n" << endl;
	_getch();
}

void Automovil::imprimirStats(bool inicio) {
	int i = 0;

	static int anterior = 0;

	int ahora = (int)time(0);
	static int conteo = 0;
	static int fps = 0;
	conteo += 1;
	if (ahora - anterior > 1) {
		fps = conteo;
		conteo = 0;
		anterior = ahora - 1;
	}
	cout << "Estado del Automovil" << endl; i++;
	cout << "______________________\n" << endl; i += 2;
	cout << "Pos x: " << round(pos.x * 100) / 100 << " m\t\t" << endl; i++;
	//cout << "Pos y: " << round(pos.y*100)/100 << " m\t\t" << endl; i++;
	cout << "Pos z: " << round(pos.z * 100) / 100 << " m\t\t" << endl; i++;
	cout << "Velocidad: " << vL * 3600 / 1000 << " km/h \t\t" << endl; i++;
	cout << "Rotacion : " << rot << " grados\t\t" << endl; i++;
	//cout << "Rot derrape: " << rotD << " grados\t\t" << endl; i++;
	//cout << "Acelerar: " << *parentAcelerar << "\t\t" << endl; i++;
	//cout << "Giro iz : " << *parentGiroIzquierda << "\t\t" << endl; i++;
	//cout << "Giro der: " << *parentGiroDerecha << "\t\t" << endl; i++;
	//cout << "Freno   : " << *parentFreno<< "\t\t" << endl; i++;
	//cout << "Retroceder : " << *parentRetroceder << "\t\t" << endl; i++;
	//cout << "FrenoMano  : " << *parentFrenoDeMano<< "\t\t" << endl; i++;
	cout << "FPS: " << fps << "\t\t" << endl; i++;
	static CONSOLE_SCREEN_BUFFER_INFO coninfo;
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &coninfo);
	coninfo.dwCursorPosition.Y -= i;    // move up one line
	coninfo.dwCursorPosition.X = 0;    // move to the right the length of the word
	SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
}

void Automovil::dibujarAutomovil() {
	glPushMatrix();
	glPushAttrib(GL_FRONT);

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot + rotD, 0, 1, 0);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
		pintura.actualizarGlMaterialfv();
		reflejo.actualizar();
		glCallList(mesh);
	glPopAttrib();

	glPushMatrix();
	if (*parentFrenoDeMano || *parentFreno || *parentRetroceder) {
		lucesTraserasMaterial.emision = Vec4(1, 0, 0, 1);
	}
	else
		lucesTraserasMaterial.emision = Vec4(0, 0, 0, 1);
	lucesTraserasMaterial.actualizarGlMaterialfv();
	glCallList(lucesTraseras);
	glScalef(1, 1, -1);
	glCallList(lucesTraseras);
	glPopMatrix();

	lucesDelanteras.actualizarGlLightfv();

	glPopAttrib();
	glPopMatrix();
}