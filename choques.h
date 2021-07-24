#ifndef _CHOQUES_H_
#define _CHOQUES_H_

#define CANT_NIVELES 6

#include "obstaculo.h"
#include "lectura.h"
#include "lista.h"
#include "poligono.h"
#include "config.h"
#include "operaciones.h"


bool choque(obstaculo_t *obstaculo, float *cx, float *cy, float *vx, float *vy, size_t *poder);
void activar_poder(float *cx, float *cy, lista_t *lista);
void verificar_choques(lista_t *lista, size_t *naranja);

#endif