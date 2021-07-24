#ifndef _OBSTACULO_H_
#define _OBSTACULO_H_

#include <stddef.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "lectura.h"
#include "poligono.h"
#include "config.h"
#include "operaciones.h"
#include "main.h" //REVISAR INCLUSION, ES SOLO PARA DT


typedef struct obstaculo {
	//bool impactado;
	size_t impactos;
	geometria_t geometria;
	movimiento_t movimiento;
	color_t color;
	poligono_t *poligono;
	size_t n_parametros;
	int16_t parametros[3];
} obstaculo_t;

obstaculo_t *crear_obstaculo(geometria_t geometria, movimiento_t movimiento, color_t color, poligono_t *poligono, size_t n_parametros, int16_t *parametros);
void destruir_obstaculo(obstaculo_t *obstaculo);
void obstaculo_impactar(obstaculo_t *obstaculo);
//obstaculo_t *obstaculo_crear_desde_archivo(FILE *f);
bool consultar_impacto(obstaculo_t *obstaculo);

void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo);
void dibujar_circulo(SDL_Renderer *renderer, poligono_t *obstaculo);
void dibujar_rectangulo(SDL_Renderer *renderer, poligono_t *obstaculo);
void dibujar_poligono(SDL_Renderer *renderer, poligono_t *obstaculo);

extern void (*dibujar_geometria[])(SDL_Renderer *renderer, poligono_t *obstaculo);

void mover(obstaculo_t *obstaculo);

obstaculo_t *crear_atrapabolas(void);

#endif