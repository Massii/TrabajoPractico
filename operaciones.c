#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include "operaciones.h"


void trasladar(float poligono[][2], size_t n, float dx, float dy) {
  for(size_t i = 0; i < n; i++) {
    poligono[i][0] += dx;
    poligono[i][1] += dy;
  }
}

float prod_vectorial(float v1[2], float v2[2]){
	return(v1[0]*v2[1] - v2[0]*v1[1]);
}

float producto_interno(float ax, float ay, float bx, float by){
  return(ax*bx + ay*by);
}

void rotar(float poligono[][2], size_t n, double rad) {
  float coseno = cos(rad);
  float seno = sin(rad);
  float x,y;

  for(size_t i = 0; i<n; i++) {
    x = poligono[i][0];
    y = poligono[i][1];
    poligono[i][0] = x * coseno - y * seno;
    poligono[i][1] = x * seno + y * coseno;
  }
}

void rotar_eje(float poligono[][2], size_t n, double rad, double x_ax, double y_ax){
  trasladar(poligono, n, -x_ax, -y_ax);
  rotar(poligono, n, rad);
  trasladar(poligono, n, x_ax, y_ax);
}

double computar_velocidad(double vi, double a, double dt){
  return(vi + a*dt);
}

double computar_posicion(double pi, double vi, double dt){
  return(pi + vi*dt);
}


double distancia(float xp, float yp, float xi, float yi) {
  return (sqrt((xp-xi)*(xp-xi) + (yp-yi)*(yp-yi)));
}

/*
Para encontrar la distancia de un punto a una recta se proyecta el punto
ortogonalmente sobre la recta.
El producto [(X.P) / (X.X)] X es la proyección del punto P sobre X.
El coeficiente entre corchetes será la proporción de P sobre X.
Como estamos trabajando con segmentos de recta, si el coeficiente es menor a
cero o mayor a uno nos caímos del segmento.
*/
void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y) {
    float ax = xp - x0;
    float ay = yp - y0;
    float bx = x1 - x0;
    float by = y1 - y0;

    float alfa = producto_interno(ax, ay, bx, by) / producto_interno(bx, by, bx, by);

    if(alfa <= 0) {
        *x = x0;
        *y = y0;
    }
    else if(alfa >= 1) {
        *x = x1;
        *y = y1;
    }
    else {
        *x = alfa * bx + x0;
        *y = alfa * by + y0;
    }
}

/*
Reflejamos según P' = P - 2 D(P.D)
*/
void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy) {
    float proy = producto_interno(norm_x, norm_y, *vx, *vy);

    if(proy >= 0)
        return;

    *vx -= 2 * norm_x * proy;
    *vy -= 2 * norm_y * proy;

    // Además empujamos a la bola hacia afuera para que no se pegue
    *cx += norm_x * 0.1;
    *cy += norm_y * 0.1;
}

