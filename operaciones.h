#ifndef _OPERACIONES_H_
#define _OPERACIONES_H_

#include "config.h"

double computar_velocidad(double vi, double a, double dt);
double computar_posicion(double pi, double vi, double dt);

void trasladar(float poligono[][2], size_t n, float dx, float dy);
void rotar(float poligono[][2], size_t n, double rad);
void rotar_eje(float poligono[][2], size_t n, double rad, double x_ax, double y_ax);

float prod_vectorial(float vector1[], float vector2[]);
float producto_interno(float ax, float ay, float bx, float by);


void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y);
void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy);

double distancia(float xp, float yp, float xi, float yi);

#endif