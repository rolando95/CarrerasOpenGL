#ifndef TOOLS
#define TOOLS V1.0
/*
	Refactory general ..... R.Vivo',2013
	Ampliacion colores .... R.Vivo',2014
	Bugs rad() y deg() .... R.Vivo',2014
	Texto en WCS y DCS .... R.Vivo',2015

	Adaptacion al proyecto de carreras... Rolando A. Rosales J., 2018
*/

#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <freeimage/FreeImage.h>

using namespace std;

#define PI 3.1415926
#ifndef max
#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
#endif
#define signo(a) (a<0?-1:1)
#define deg(a) (a*180/PI)
#define rad(a) (a*PI/180)


const GLfloat BLANCO[] = { 1,1,1,1 };
const GLfloat NEGRO[] = { 0,0,0,1 };
const GLfloat GRISCLARO[] = { 0.7,0.7,0.7,1 };
const GLfloat GRISOSCURO[] = { 0.2,0.2,0.2,1 };
const GLfloat ROJO[] = { 1,0,0,1 };
const GLfloat VERDE[] = { 0,1,0,1 };
const GLfloat AZUL[] = { 0,0,1,1 };
const GLfloat AMARILLO[] = { 1,1,0,1 };
const GLfloat BRONZE[] = { 140.0 / 255,120.0 / 255,83.0 / 255,1 };
const GLfloat BRONCE[] = { 140.0 / 255,120.0 / 255,83.0 / 255,1 };
const GLfloat MARINO[] = { 0,0,0.5,1 };
const GLfloat ORO[] = { 218.0 / 255,165.0 / 255,32.0 / 255,1 };


void planoXY(int resolucion = 10);
/* resolucion: numero de divisiones opcional del lado (por defecto 10)
   Dibuja el cuadrado unidad (-0.5,-0.5)(0.5,0.5) con
   "resolucion" divisiones usando strips horizontales.
   Las normales se generan como (0,0,1)            */

void quad(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int M = 10, int N = 10);
/* v0,v1,v2,v3: vertices del quad
   NxM: resolucion opcional (por defecto 10x10)
   Dibuja el cuadrilatero de entrada con resolucion MxN y normales.
   Se asume counterclock en la entrada                            */

void quadtex(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3],
	GLfloat smin = 0, GLfloat smax = 1, GLfloat tmin = 0, GLfloat tmax = 1,
	int M = 10, int N = 10);
/* v0,v1,v2,v3: vertices del quad
   (smin,tmin),(smax,tmax): coordenadas de textura de v0 y v2 respectivamente
   NxM: resolucion opcional (por defecto 10x10)
   Dibuja el cuadrilatero de entrada con resolucion MxN, normales y ccordenadas
   de textura seg�n rango dado.
   Se asume antihorario en la entrada para caras frontales                      */

void ejes();
/* Dibuja unos ejes de longitud 1 y una esferita en el origen */

void texto(unsigned int x, unsigned int y, char *text, const GLfloat *color = ROJO, void *font = GLUT_BITMAP_HELVETICA_18, bool WCS = true);
/* Muestra en la posicion (x, y) del viewport la cadena de texto con la fuente y el color indicados
   Los tres ultimos parametros son opcionales.
   @WCS: x,y en coordenadas del mundo -true- o del viewport -false-.
   Ver en glut.h las posibles fuentes que se pueden utilizar.
   Por ejemplo: texto(30, 30, "Hola mundo"); */

void loadImageFile(char* nombre);
/* Uso de FreeImage para cargar la imagen en cualquier formato
   nombre: nombre del fichero con extension en el mismo directorio que el proyecto o con su path completo */

void saveScreenshot(char* nombre, int ancho, int alto);
/* Utiliza FreeImage para grabar un png
   nombre: Nombre del fichero con extension p.e. salida.png
   ancho: Ancho del viewport en pixeles
   alto: Alto del viewport en pixeles */

void texturarFondo();
/* Carga como fondo del area de dibujo la textura corriente.
   Debe llamarse inmediatamente despues del glClear()       */

#endif