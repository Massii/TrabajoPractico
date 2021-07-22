#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

#define MIN_X 77
#define MAX_X 721
#define MIN_Y 52
#define MAX_Y 592

#define CANON_X 401
#define CANON_Y 71
#define CANON_LARGO 93
#define CANON_MAX 1.6790669176553528

#define RESOLUCION 20
#define BOLA_RADIO 6
#define BOLA_VI 450

#define JUEGO_FPS 200

#define G 600
#define PLASTICIDAD 0.8
#define ROZAMIENTO 0.9999

// Operaciones

double computar_velocidad(double vi, double a, double dt);
double computar_posicion(double pi, double vi, double dt);

void trasladar(float poligono[][2], size_t n, float dx, float dy);
void rotar(float poligono[][2], size_t n, double rad);
void rotar_eje(float poligono[][2], size_t n, double rad, double x_ax, double y_ax);

float prod_vectorial(float vector1[], float vector2[]);
float producto_interno(float ax, float ay, float bx, float by);


void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y);
void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy);

double distancia(float xp, float yp, float xi, float yi);

// Poligono

typedef struct {
  float (*vertices)[2];
  size_t n;
} poligono_t;

poligono_t *poligono_crear(float vertices[][2], size_t n);
void poligono_destruir(poligono_t *pol);
size_t poligono_cantidad_vertices(const poligono_t *pol);
bool poligono_obtener_vertice(const poligono_t *pol, size_t pos, float *x, float *y);
poligono_t *poligono_clonar(const poligono_t *pol);
bool poligono_agregar_vertice(poligono_t *pol, float x, float y);
poligono_t *circular(size_t resolucion, size_t radio, size_t x, size_t y);

bool punto_en_triangulo(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);
bool punto_en_poligono(float poligono[][2], size_t n, float px, float py); 

double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y);

//Lectura 

#define MASK_COLOR 0xC0 // 1100 0000
#define MASK_MOV 0x30 // 0011 0000
#define MASK_GEO 0x0F // 0000 1111

#ifndef color_t
typedef enum color {COLOR_AZUL, COLOR_NARANJA, COLOR_VERDE, COLOR_GRIS} color_t;
#endif

#ifndef movimiento_t
typedef enum movimiento {MOV_INMOVIL, MOV_CIRCULAR, MOV_HORIZONTAL} movimiento_t;
#endif

#ifndef geometria_t
typedef enum geometria {GEO_CIRCULO, GEO_RECTANGULO, GEO_POLIGONO} geometria_t;
#endif


const char *colores[] = {
	[COLOR_AZUL] = "Azul", [COLOR_NARANJA] = "Naranja", [COLOR_VERDE] = "Verde", [COLOR_GRIS] = "Gris"
};


const char *nombres_movimiento[] = {
	[MOV_INMOVIL] = "Inmóvil", [MOV_CIRCULAR] = "Circular", [MOV_HORIZONTAL] = "Horizontal"
};

const char *nombres_geometria[] = {
	[GEO_CIRCULO] = "Círculo", [GEO_RECTANGULO] = "Rectángulo", [GEO_POLIGONO] = "Polígono"
};



bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria);


bool leer_movimiento_inmovil(FILE *f, int16_t parametros[], size_t *n_parametros);
bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros);
bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros);
bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros);
bool (*movimientos[])(FILE *f, int16_t parametros[], size_t *n_parametros) = {
	[MOV_INMOVIL] = leer_movimiento_inmovil, 
	[MOV_CIRCULAR] = leer_movimiento_circular, 
	[MOV_HORIZONTAL] = leer_movimiento_horizontal
};

poligono_t *leer_geometria_circulo(FILE *f);
poligono_t *leer_geometria_rectangulo(FILE *f);
poligono_t *leer_geometria_poligono(FILE *f);
poligono_t *leer_geometria(FILE*f, geometria_t geometria);
poligono_t *(*geometrias[])(FILE *f) = {leer_geometria_circulo, leer_geometria_rectangulo, leer_geometria_poligono};

//Obstaculo 
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

void dibujar_circulo(SDL_Renderer *renderer, obstaculo_t *obstaculo);

void dibujar_rectangulo(SDL_Renderer *renderer, obstaculo_t *obstaculo);

void dibujar_poligono(SDL_Renderer *renderer, obstaculo_t *obstaculo);


void (*dibujar_geometria[])(SDL_Renderer *renderer, obstaculo_t *obstaculo) = {
	[GEO_CIRCULO] = dibujar_circulo, 
	[GEO_RECTANGULO] = dibujar_rectangulo, 
	[GEO_POLIGONO] = dibujar_poligono,
};
