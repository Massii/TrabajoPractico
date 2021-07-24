#ifndef _POLIGONO_H_
#define _POLIGONO_H_

#include "operaciones.h"
#include "config.h"

typedef struct {
  float (*vertices)[2];
  size_t n;
} poligono_t;

poligono_t *poligono_crear(float vertices[][2], size_t n);
void poligono_destruir(poligono_t *pol);
size_t poligono_cantidad_vertices(const poligono_t *pol);
bool poligono_obtener_vertice(const poligono_t *pol, size_t pos, float *x, float *y);
poligono_t *poligono_clonar(const poligono_t *pol);
bool poligono_agregar_vertice(poligono_t *pol, float x, float y);
poligono_t *circular(size_t resolucion, size_t radio, size_t x, size_t y);

bool punto_en_triangulo(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);
bool punto_en_poligono(float poligono[][2], size_t n, float px, float py); 

double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y);

#endif