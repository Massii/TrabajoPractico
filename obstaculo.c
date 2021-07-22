#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "obstaculo.h"

obstaculo_t *crear_obstaculo(geometria_t geometria, movimiento_t movimiento, color_t color, poligono_t *poligono) {
  obstaculo_t *obstaculo = malloc(sizeof(obstaculo_t)); 
  if(obstaculo == NULL)
    return NULL;

  obstaculo->geometria = geometria;
  obstaculo->movimiento = movimiento;
  obstaculo->color = color;
  obstaculo->poligono = poligono;

  obstaculo->impactado = false;
  return obstaculo;
}

void destruir_obstaculo(obstaculo_t *obstaculo) {
  poligono_destruir(obstaculo->poligono);
  free(obstaculo);
}


bool consultar_impacto(obstaculo_t *obstaculo) {
  return obstaculo->impactado;
}

void obstaculo_impactar(obstaculo_t *obstaculo) {
  obstaculo->impactado = true;
}

obstaculo_t *obstaculo_crear_desde_archivo(FILE *f) {
  geometria_t geometria;
  movimiento_t movimiento;
  color_t color;

  leer_encabezado(f, &color, &movimiento, &geometria);

  poligono_t *poligono = leer_geometria(f, geometria);
  obstaculo_t *obstaculo = crear_obstaculo(geometria, movimiento, color, poligono);
  return obstaculo;
}


