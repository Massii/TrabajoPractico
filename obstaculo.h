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

//SDL2

void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo);

void dibujar_circulo(SDL_Renderer *renderer, poligono_t *obstaculo);

void dibujar_rectangulo(SDL_Renderer *renderer, poligono_t *obstaculo);

void dibujar_poligono(SDL_Renderer *renderer, poligono_t *obstaculo);


void (*dibujar_geometria[])(SDL_Renderer *renderer, poligono_t *obstaculo) = {
	[GEO_CIRCULO] = dibujar_circulo, 
	[GEO_RECTANGULO] = dibujar_rectangulo, 
	[GEO_POLIGONO] = dibujar_poligono,
};

#endif