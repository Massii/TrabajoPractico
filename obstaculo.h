#ifndef _OBSTACULO_H_
#define _OBSTACULO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "lectura.h"
#include "poligono.h"

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

void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo);
void dibujar_circulo(SDL_Renderer *renderer, poligono_t *obstaculo);
void dibujar_rectangulo(SDL_Renderer *renderer, poligono_t *obstaculo);
void dibujar_poligono(SDL_Renderer *renderer, poligono_t *obstaculo);

extern void (*dibujar_geometria[])(SDL_Renderer *renderer, poligono_t *obstaculo);

#endif