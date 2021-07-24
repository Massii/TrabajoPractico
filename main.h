#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "lista.h"

#define DT (1.0 / JUEGO_FPS)

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y);
void lectura(FILE *f, SDL_Renderer *renderer, lista_t *lista);

#endif