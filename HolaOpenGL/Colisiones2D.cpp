#include "Juego.h"

void Colisiones2D::obtenerPuntosTerreno(Vec3 vterreno[MAX][4], int vti)
{
	ti = vti;
	for (auto x = 0; x < ti; x++)
		for (auto y = 0; y < 4; y++){
			terreno[x][y] = vterreno[x][y];
		}
}

void Colisiones2D::asignarMeshColision(float vRadio)
{
	radio = vRadio;
}

void Colisiones2D::parentarObjeto(Vec3 * objPos, float * objRot, float * objVelocidad)
{
	parentPos = objPos;
	parentRot = objRot;
	parentVelocidad = objVelocidad;
}

void Colisiones2D::parentarExtraRotObjeto(float *objExtraRot) {
	parentExtraRot = objExtraRot;
}

void Colisiones2D::colisionar(int pistaTerreno)
{
	int pos = 0;
	int lado = 0; //0-> parte baja, 3-> parte alta
	float cerca = modulo(terreno[0][1], *parentPos);
	float mod;

	//Calcula el punto más cercano al automovil
	if (pistaTerreno > -1 && pistaTerreno < ti) {
		pos = pistaTerreno;
		cerca = modulo(terreno[pos][0], *parentPos);
		mod = modulo(terreno[pos][3], *parentPos);
		if (mod < cerca) { lado = 3, cerca = mod;}
	}
	else {
		for (auto i = 0; i < ti; i++) {
			for (auto j = 1; j <= 2; j+=1) {
				mod = modulo(terreno[i][j], *parentPos);
				if (mod < cerca) {
					pos = i; lado = j /*(j-1)*3 /*0 o 3*/; cerca = mod;
				}
			}
		}
	}

	//Calcula si el automovil está colisionando en el plano XZ
	/*
	^ x (y)
	|
	|
	|______> z (x)
	*/
	float x = terreno[(pos + 1) % ti][lado].z;
	float x0 = terreno[pos][lado].z;
	float y = terreno[(pos + 1) % ti][lado].x;
	float y0 = terreno[pos][lado].x;

	float h = parentPos->z;
	float k = parentPos->x;

	float mx = x - x0;
	float my = y - y0;

	int n=0;
	float m, b;

	//cout << "("<<x0 << " " << y0 << ")( " << x << " " << y << ")( " << my/mx << " "<< y - my/mx * x<<") c(" << h << " " << k<< ") --";
	if (mx == 0 || abs(my / mx) > 1) {
		float tmpy0=y0, tmpy=y;
		y0 = x0;
		y = x;
		x0 = -tmpy0;
		x = -tmpy;

		float tmp = k;
		k = h;
		h = -tmp;
		n = 1;

		m = -mx / my;
		b = y - m * x;
	}
	else {
		m = my / mx;
		b = y - m * x;
	}

	//cout << "("<<x0 << " " << y0 << ")( " << x << " " << y << ")( " << m << " " << b << ") c(" << h << " " << k<<")";
	//Se detectan problemas con posiciones de pendientes cercanas a inf
	if (pos != 103 && pos != 111 && pos != 116 && pos != 126){

		//Circunferencia
		float r = radio;
		float A = -2 * h;
		float B = -2 * k;
		float C = h * h + k * k - r * r;

		//glPushMatrix(); glTranslatef(terreno[pos][lado].x, terreno[pos][lado].y, terreno[pos][lado].z); glutSolidCube(3); glPopMatrix();
		//pushAtributosObjetos(); glTranslatef(parentPos->x, parentPos->y, parentPos->z); glScalef(r, r, r); glRotatef(-90, 1, 0, 0); if (n == 0)glutSolidCylinder(1, 1, 8, 1); else glutSolidCone(1, 1, 8, 1); popAtributosObjetos();

		//static int posC = 0;
		//cout << posC <<" "<< mx <<" ";

		//Ecuacion de la recta
		//cout << "y= " << round(m * 10) / 10 << "x +" << round(b * 10) / 10 << " -- ";

		//Ecuacion de la circunferencia
		//cout << "x^2 +y^2 +" << round(A*10)/10 << "x +" << round(B*10)/10 << "y +" << round(C*10)/10 << " =0 ";

		//Interseccion		
		float a = 1 + m * m;
		float b1 = 2*m*b + A + B*m;
		float c = b*b + b*B + C;

		float coef = b1 * b1 - 4 * a*c;
		if ( coef >= 0) {
			float xiz = (-b1 - sqrt(coef)) / (2 * a);
			float xder = (-b1 + sqrt(coef)) / (2 * a);
			if (h-r < xiz && h+r > xder){//( (parentPos->z - r) < xiz &&  (parentPos->z + r) > xder ) {
				//cout <<">> "<< lado << " COLISION ";
				//posC = pos;
				*parentVelocidad = 0;
				parentPos->x = (terreno[pos][2].x + terreno[pos][1].x) / 2;
				parentPos->z = (terreno[pos][2].z + terreno[pos][1].z) / 2;
				*parentRot = anguloDeg(terreno[pos+1][2] - terreno[pos][2], (1, 0, 0)) * signoV(terreno[pos+1][2] - terreno[pos][2]).z * -1;
				*parentExtraRot = 0;
			}
		}
		else {
			//cout <<">>"<< coef << " NO COLISION ";
		}
	}
	
}
