#ifndef _CHOQUES_H_
#define _CHOQUES_H_

#define CANT_NIVELES 7
#define MAX_BOLITAS 13

#include "obstaculo.h"
#include "lectura.h"
#include "lista.h"
#include "poligono.h"
#include "config.h"
#include "operaciones.h"


bool choque(obstaculo_t *obstaculo, float *cx, float *cy, float *vx, float *vy, size_t *poder, double *puntos, double *contador_naranjas);
void activar_poder(float *cx, float *cy, lista_t *lista, double *puntos, double *contador_naranjas);
void verificar_choques(lista_t *lista, size_t *naranja);

#endif