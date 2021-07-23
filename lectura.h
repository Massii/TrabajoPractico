#ifndef _LECTURA_H_
#define _LECTURA_H_

#include <stddef.h>
#include <stdint.h>

#include "poligono.h"

#define MASK_COLOR 0xC0 // 1100 0000
#define MASK_MOV 0x30 // 0011 0000
#define MASK_GEO 0x0F // 0000 1111

typedef enum color {COLOR_AZUL, COLOR_NARANJA, COLOR_VERDE, COLOR_GRIS} color_t;
typedef enum movimiento {MOV_INMOVIL, MOV_CIRCULAR, MOV_HORIZONTAL} movimiento_t;
typedef enum geometria {GEO_CIRCULO, GEO_RECTANGULO, GEO_POLIGONO} geometria_t;


extern const char *colores[];
extern const char *nombres_movimiento[];
extern const char *nombres_geometria[];


bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria);

bool leer_movimiento_inmovil(FILE *f, int16_t parametros[], size_t *n_parametros);
bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros);
bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros);
bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros);

extern bool (*movimientos[])(FILE *f, int16_t parametros[], size_t *n_parametros);

poligono_t *leer_geometria_circulo(FILE *f);
poligono_t *leer_geometria_rectangulo(FILE *f);
poligono_t *leer_geometria_poligono(FILE *f);
poligono_t *leer_geometria(FILE*f, geometria_t geometria);

extern poligono_t *(*geometrias[])(FILE *f);

#endif