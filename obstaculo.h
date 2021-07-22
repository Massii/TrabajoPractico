#ifndef _OBSTACULOS_H_
#define _OBSTACULOS_H_


#include "lectura.h"
#include "poligono.h"
#include "config.h"

typedef struct obstaculo {
	bool impactado;
	geometria_t geometria;
	movimiento_t movimiento;
	color_t color;
	poligono_t *poligono;
} obstaculo_t;


obstaculo_t *crear_obstaculo(geometria_t geometria, movimiento_t movimiento, color_t color, poligono_t *poligono);

void destruir_obstaculo(obstaculo_t *obstaculo);

void obstaculo_impactar(obstaculo_t *obstaculo);

obstaculo_t *obstaculo_crear_desde_archivo(FILE *f);

bool consultar_impacto(obstaculo_t *obstaculo);

#endif