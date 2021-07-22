#ifndef _SDL2_H_
#define _SDL2_H_


#include "config.h"
#include "operaciones.h"
#include "obstaculo.h"
#include "lectura.h"
#include "poligono.h"

void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo);

void dibujar_circulo(SDL_Renderer *renderer, obstaculo_t *obstaculo);

void dibujar_rectangulo(SDL_Renderer *renderer, obstaculo_t *obstaculo);

void dibujar_poligono(SDL_Renderer *renderer, obstaculo_t *obstaculo);


void (*dibujar_geometria[])(SDL_Renderer *renderer, obstaculo_t *obstaculo) = {
	[GEO_CIRCULO] = dibujar_circulo, 
	[GEO_RECTANGULO] = dibujar_rectangulo, 
	[GEO_POLIGONO] = dibujar_poligono,
};

#endif