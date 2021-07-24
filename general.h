#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

#define JUEGO_FPS 188

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
	size_t n_parametros;
	int16_t parametros[3];
} obstaculo_t;


obstaculo_t *crear_obstaculo(geometria_t geometria, movimiento_t movimiento, color_t color, poligono_t *poligono, size_t n_parametros, int16_t *parametros);

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




//// LISTAS


struct lista;
struct lista_iter;

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/*
 * Primitivas de lista simplemente enlazada
 */

// Crea una lista
// Post: Se devuelve una nueva lista.
lista_t *lista_crear(void);

// Verifica si la lista es vacía
// Pre: la lista fue creada
bool lista_esta_vacia(const lista_t *lista);

// Agrega un elemento al principio de la lista
// Pre: la lista fue creada
// Post: se agregó un nodo con el dato recibido, al principio de la lista
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un elemento al final de la lista
// Pre: la lista fue creada
// Post: se agregó un nodo con el dato recibido, al final de la lista
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista
// Pre: la lista fue creada
// Post: se eliminó el primer nodo de la lista, se devuelve el dato contenido.
// En caso de lista vacía devuelve NULL.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el contenido de dato del primer nodo
// Pre: la lista fue creada
// Post: devuelve el valor almacenado en el primer dato, en caso de lista
// vacía devuelve NULL.
void *lista_ver_primero(const lista_t *lista);

// Obtener largo
// Pre: la lista fue creada
// Post: devuelve la cantidad de elementos almacenados
size_t lista_largo(const lista_t *lista);

// Destruye la lista
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/*
 * Primitivas de iteración
 */

// Obtiene un iterador de la lista
// Pre: la lista fue creada
// Post: se devuelve un nuevo iterador
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posición en la lista
// Pre: el iterador fue creado
// Post: se avanzó la posición actual en el iterador. Devuelve false si ya
// está al final de la lista
bool lista_iter_avanzar(lista_iter_t *iter);

// Almacena en dato el valor que se encuentra en la posición actual
// Pre: el iterador fue creado
// Post: se almacenó el dato actual en dato. Devuelve false si está al final
// de la lista
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Verifica si ya está al final de la lista
// Pre: el iterador fue creado
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador de la lista
void lista_iter_destruir(lista_iter_t *iter);

/*
 * Primitivas de listas junto con iterador
 */

// Agrega un elemento en la posición actual
// Pre: el iterador fue creado
// Post: se insertó un nuevo nodo antes de la posición actual, quedando este
// nuevo nodo como posición actual
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento de la posición actual
// Pre: el iterador fue creado
// Post: se eliminó el nodo que se encontraba en la posición actual indicada por el
// iterador. Devuelve NULL si el iterador está al final de la lista
void *lista_iter_borrar(lista_iter_t *iter);

/*
 * Primitivas de iterador interno
 */

// Iterador interno de la lista.
// Recorre la lista y para cada elemento de la misma llama a
// la función visitar, pasándole por parámetro el elemento de la lista
// (dato) y el parámetro extra.
// El parámetro extra sirve para que la función visitar pueda recibir
// información adicional. Puede ser NULL.
// La función visitar debe devolver true si se debe seguir iterando,
// false para cortar la iteración. No puede agregar ni quitar elementos
// a la lista).
// Pre: la lista fue creada.
// Post: se llamó a la función visitar una vez por cada elemento de la lista,
// en orden.
void lista_iterar(lista_t *lista,
                  bool (*visitar)(void *dato, void *extra),
                  void *extra);

#endif  // LISTA_H