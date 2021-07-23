#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "obstaculo.h"
#include "lectura.h"
#include "poligono.h"
#include "config.h"


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

//si no funciona pasar al main
void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo) {
  switch(obstaculo->color) {
      case(COLOR_AZUL): {
              SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
              break;
            }
      case(COLOR_NARANJA):{
              SDL_SetRenderDrawColor(renderer, 0xFF, 0xC0, 0x00, 0x00);
              break;
            }
      case(COLOR_GRIS):{ 
              SDL_SetRenderDrawColor(renderer, 0xC0, 0xC0, 0xC0, 0x00);
              break;
            }
      case(COLOR_VERDE): {
              SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
              break;
            }
  }
  dibujar_geometria[obstaculo->geometria](renderer, obstaculo->poligono);
}


void dibujar_circulo(SDL_Renderer *renderer, poligono_t *circulo) {
    for(size_t i = 0; i < circulo->n-1; i++) {
        SDL_RenderDrawLine(renderer, circulo->vertices[i][0], circulo->vertices[i][1], circulo->vertices[i+1][0], circulo->vertices[i+1][1]);
    }
}


void dibujar_rectangulo(SDL_Renderer *renderer, poligono_t *rectangulo) {
    for(size_t i = 0; i < 3; i++) {
            SDL_RenderDrawLine(renderer, rectangulo->vertices[i][0], rectangulo->vertices[i][1], rectangulo->vertices[i+1][0], rectangulo->vertices[i+1][1]);
        }
        SDL_RenderDrawLine(renderer, rectangulo->vertices[3][0], rectangulo->vertices[3][1], rectangulo->vertices[0][0], rectangulo->vertices[0][1]);
}

void dibujar_poligono(SDL_Renderer *renderer, poligono_t *obstaculo) {
    size_t i = 0;

    for(i = 0; i < obstaculo->n-1; i++) {
            SDL_RenderDrawLine(renderer, obstaculo->vertices[i][0], obstaculo->vertices[i][1], obstaculo->vertices[i+1][0], obstaculo->vertices[i+1][1]);
        }
        SDL_RenderDrawLine(renderer, obstaculo->vertices[obstaculo->n-1][0], obstaculo->vertices[obstaculo->n-1][1], obstaculo->vertices[0][0], obstaculo->vertices[0][1]);
}

void (*dibujar_geometria[])(SDL_Renderer *renderer, poligono_t *obstaculo) = {
  [GEO_CIRCULO] = dibujar_circulo, 
  [GEO_RECTANGULO] = dibujar_rectangulo, 
  [GEO_POLIGONO] = dibujar_poligono,
};