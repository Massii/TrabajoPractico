#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "lista.h"

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y);
void lectura(int argc, char *argv[], SDL_Renderer *renderer, lista_t *lista);

#endif