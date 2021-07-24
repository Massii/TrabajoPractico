#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "general.h"


//LISTAS
typedef struct nodo {
    void *dato;
    struct nodo *prox;
} nodo_t;

struct lista {
    nodo_t *primero;
    nodo_t *ultimo;
    size_t largo;
};

struct lista_iter {
    lista_t *lista;
    nodo_t *ant;
    nodo_t *act;
};

lista_t *lista_crear() {
    lista_t *lista = malloc(sizeof(lista_t));

    if (!lista)
        return NULL;

    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista && lista->largo == 0;
}

static nodo_t *crear_nodo(void *dato, nodo_t *prox) {
    nodo_t *nuevo = malloc(sizeof(nodo_t));

    if (!nuevo)
        return NULL;

    nuevo->dato = dato;
    nuevo->prox = prox;
    return nuevo;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nuevo = crear_nodo(dato, lista->primero);

    if (!nuevo)
        return false;

    lista->largo++;
    lista->primero = nuevo;

    if (!lista->ultimo)
        lista->ultimo = nuevo;

    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nuevo = crear_nodo(dato, NULL);

    if (!nuevo)
        return false;

    if (lista->ultimo)
        lista->ultimo->prox = nuevo;

    lista->ultimo = nuevo;

    if (!lista->primero)
        lista->primero = nuevo;

    lista->largo++;

    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (!lista->primero)
        return NULL;

    nodo_t *aux = lista->primero;
    lista->primero = aux->prox;
    void *dato = aux->dato;
    free(aux);

    if (lista->ultimo == aux)
        lista->ultimo = NULL;

    lista->largo--;

    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista))
        return NULL;

    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (lista_esta_vacia(lista))
        return NULL;

    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
    while (lista->primero) {
        nodo_t *nodo = lista->primero;
        if (destruir_dato)
            destruir_dato(nodo->dato);
        lista->primero = nodo->prox;
        free(nodo);
    }
    free(lista);
}


lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));

    if (!iter)
        return NULL;

    iter->lista = lista;
    iter->ant = NULL;
    iter->act = lista->primero;

    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter))
        return false;

    iter->ant = iter->act;
    iter->act = iter->act->prox;

    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (lista_iter_al_final(iter))
        return NULL;

    return iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->act == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}


bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t *nuevo = crear_nodo(dato, iter->act);

    if (!nuevo)
        return false;

    // Primera posición
    if (iter->ant == NULL) {
        iter->lista->primero = nuevo;
    }
    else {
        iter->ant->prox = nuevo;
    }

    // Última posición
    if (iter->act == NULL) {
        iter->lista->ultimo = nuevo;
    }
    iter->act = nuevo;
    iter->lista->largo++;

    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (!iter->act)
        return NULL;

    nodo_t *nodo = iter->act;
    void *dato = nodo->dato;

    if (!iter->ant) {
        iter->lista->primero = nodo->prox;
    }
    else {
        iter->ant->prox = nodo->prox;
    }

    if (!nodo->prox)
        iter->lista->ultimo = iter->ant;

    iter->act = nodo->prox;
    iter->lista->largo--;
    free(nodo);

    return dato;
}


void lista_iterar(lista_t *lista,
                  bool (*visitar)(void *, void *),
                  void *extra) {
    nodo_t *nodo = lista->primero;
    while (nodo) {
        if (!visitar(nodo->dato, extra))
            return;
        nodo = nodo->prox;
    }
}

//////////////////////////////////// Operaciones

void trasladar(float poligono[][2], size_t n, float dx, float dy) {
  for(size_t i = 0; i < n; i++) {
    poligono[i][0] += dx;
    poligono[i][1] += dy;
  }
}


float prod_vectorial(float v1[2], float v2[2]){
	return(v1[0]*v2[1] - v2[0]*v1[1]);
}

float producto_interno(float ax, float ay, float bx, float by){
  return(ax*bx + ay*by);
}

void rotar(float poligono[][2], size_t n, double rad) {
  float coseno = cos(rad);
  float seno = sin(rad);
  float x,y;

  for(size_t i = 0; i<n; i++) {
    x = poligono[i][0];
    y = poligono[i][1];
    poligono[i][0] = x * coseno - y * seno;
    poligono[i][1] = x * seno + y * coseno;
  }
}

void rotar_eje(float poligono[][2], size_t n, double rad, double x_ax, double y_ax){
  trasladar(poligono, n, -x_ax, -y_ax);
  rotar(poligono, n, rad);
  trasladar(poligono, n, x_ax, y_ax);
}

double computar_velocidad(double vi, double a, double dt){
  return(vi + a*dt);
}

double computar_posicion(double pi, double vi, double dt){
  return(pi + vi*dt);
}


double distancia(float xp, float yp, float xi, float yi) {
  return (sqrt((xp-xi)*(xp-xi) + (yp-yi)*(yp-yi)));
}

/*
Para encontrar la distancia de un punto a una recta se proyecta el punto
ortogonalmente sobre la recta.
El producto [(X.P) / (X.X)] X es la proyección del punto P sobre X.
El coeficiente entre corchetes será la proporción de P sobre X.
Como estamos trabajando con segmentos de recta, si el coeficiente es menor a
cero o mayor a uno nos caímos del segmento.
*/
void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y) {
    float ax = xp - x0;
    float ay = yp - y0;
    float bx = x1 - x0;
    float by = y1 - y0;

    float alfa = producto_interno(ax, ay, bx, by) / producto_interno(bx, by, bx, by);

    if(alfa <= 0) {
        *x = x0;
        *y = y0;
    }
    else if(alfa >= 1) {
        *x = x1;
        *y = y1;
    }
    else {
        *x = alfa * bx + x0;
        *y = alfa * by + y0;
    }
}

/*
Reflejamos según P' = P - 2 D(P.D)
*/
void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy) {
    float proy = producto_interno(norm_x, norm_y, *vx, *vy);

    if(proy >= 0)
        return;

    *vx -= 2 * norm_x * proy;
    *vy -= 2 * norm_y * proy;

    // Además empujamos a la bola hacia afuera para que no se pegue
    *cx += norm_x * 0.1;
    *cy += norm_y * 0.1;
}

//POLIGONO

poligono_t *poligono_crear(float vertices[][2], size_t n) {
  poligono_t *pol = malloc(sizeof(poligono_t));

  if(pol == NULL) {
    return NULL;
  }

  pol->n = n;

  pol->vertices = malloc(2 * n * sizeof(float));
  if(pol->vertices == NULL) {
    free(pol);
    return NULL;
  }

  for(size_t i = 0; i < n; i++) {
    pol->vertices[i][0] = vertices[i][0];
    pol->vertices[i][1] = vertices[i][1];
  }

  return pol;
}

void poligono_destruir(poligono_t *pol) {
  free (pol->vertices);
  free (pol);
}


size_t poligono_cantidad_vertices(const poligono_t *pol) {
  return pol->n;
}


bool poligono_obtener_vertice(const poligono_t *pol, size_t pos, float *x, float *y) {
  if(pos >= pol->n)
    return false;
  
  *x = pol->vertices[pos][0];
  *y = pol->vertices[pos][1];

  return true;
}


poligono_t *poligono_clonar(const poligono_t *pol) {
  return(poligono_crear(pol->vertices, pol->n));

}

bool poligono_agregar_vertice(poligono_t *pol, float x, float y) {
  float (*aux)[2] = realloc(pol->vertices, (pol->n + 1) * 2 * sizeof(float));
  if(aux == NULL)
    return false;
  
  pol->vertices = aux;

  pol->vertices[pol->n][0] = x;
  pol->vertices[pol->n][1] = y;
  
  pol->n ++; 
  return true;
}

void poligono_invertir(poligono_t *pol) {
    float aux[2];
    for(size_t i = 0, j = pol->n-1; i < (pol->n)/2; i++, j--) {
        aux[0] = pol->vertices[i][0];
        aux[1] = pol->vertices[i][1];

        pol->vertices[i][0] = pol->vertices[j][0];
        pol->vertices[i][1] = pol->vertices[j][1];

        pol->vertices[j][0] = aux[0];
        pol->vertices[j][1] = aux[1];
    }
    return;
}


poligono_t *circular(size_t resolucion, size_t radio, size_t x, size_t y) {
    float vertices[resolucion][2];

    float angulo = 360/resolucion;

    for(size_t i = 0; i < resolucion; i++) {
      vertices[i][0] = radio*cos(i*angulo*PI/180);
      vertices[i][1] = radio*sin(i*angulo*PI/180);
    }

    trasladar(vertices, resolucion, x, y);
    poligono_t *circulo = poligono_crear(vertices, resolucion);
    
    return circulo;
}


double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y) {
    double d = 1 / 0.0;
    size_t idx = 0;

    for(size_t i = 0; i < p->n; i++) {
        float xi, yi;
        punto_mas_cercano(p->vertices[i][0], p->vertices[i][1], p->vertices[(i + 1) % p->n][0], p->vertices[(i + 1) % p->n][1], xp, yp, &xi, &yi);
        double di = distancia(xp, yp, xi, yi);

        if(di < d) {
            d = di;
            idx = i;
        }
    }

    float nx = p->vertices[(idx + 1) % p->n][1] - p->vertices[idx][1];
    float ny = p->vertices[idx][0] - p->vertices[(idx + 1) % p->n][0];
    float dn = distancia(nx, ny, 0, 0);

    nx /= dn;
    ny /= dn;

    *nor_x = nx;
    *nor_y = ny;

    return d;
}

bool punto_en_triangulo(float px, float py, float ax, float ay, float bx, float by, float cx, float cy){

  float v1[2], v2[2], v3[2], pa[2], pb[2], a, b, c;

  v1[0] = cx - ax;
  v1[1] = cy - ay;
  v2[0] = bx - ax;
  v2[1] = by - ay;
  v3[0] = cx - bx;
  v3[1] = cy - by;
  pa[0] = px - ax;
  pa[1] = py - ay;
  pb[0] = px - bx;
  pb[1] = py - by;

  a = prod_vectorial (v1, pa);
  b = prod_vectorial (pa, v2);
  c = prod_vectorial (pb, v3);

  if(a >= 0 && b >= 0 && c >= 0){
    return true;
  }else if(a <= 0 && b <= 0 && c <= 0){
    return true;
  }

  return false;
}


bool punto_en_poligono(float poligono[][2], size_t n, float px, float py){
  for (int i = 1; i < n-1; i++)
  {
    if(punto_en_triangulo(px, py, poligono[0][0], poligono[0][1], poligono[i][0], poligono[i][1], poligono[i+1][0], poligono[i+1][1]) == true){
      return true;
    }
  }

  return false;
}




////// LECTURA /////////////

bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria) {
    char encabezado;
    if(fread(&encabezado, sizeof(char), 1, f) != 1)
      return false;

    *color = (encabezado & MASK_COLOR) >> 6;
    *movimiento = (encabezado & MASK_MOV) >> 4;
    *geometria = (encabezado & MASK_GEO);

    return true;
}

bool leer_movimiento_inmovil(FILE *f, int16_t parametros[], size_t *n_parametros) {
  *n_parametros = 0;
  return true;
}

bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros) {
    *n_parametros = 3;
    return fread(parametros, sizeof(int16_t), 3, f) == 3;
}

bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros) {
    *n_parametros = 3;
    return fread(parametros, sizeof(int16_t), 3, f) == 3;
}

bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros) {
  return movimientos[movimiento](f, parametros, n_parametros);
}

poligono_t *leer_geometria_circulo(FILE *f) {
    int16_t v[3];
    if(fread(v, sizeof(int16_t), 3, f) != 3) {
      fprintf(stderr, "Error lectura\n");
      return NULL;
    }

    poligono_t *circulo = circular(RESOLUCION, v[2], v[0], v[1]);

    printf("CIRCULO: x = %d y = %d, radio = %d \n", v[0], v[1], v[2]);

    return circulo;
}

poligono_t *leer_geometria_rectangulo(FILE *f) {
    int16_t v[5];
    if(fread(v, sizeof(int16_t), 5, f) != 5) {
      fprintf(stderr, "Error lectura\n");
      return NULL;
    }
    // x, y, ancho, alto, angulo
    float vertices[][2] = {{v[2]/2, v[3]/2}, {-v[2]/2, v[3]/2}, {-v[2]/2, -v[3]/2}, {v[2]/2, -v[3]/2}};
    poligono_t *rectangulo = poligono_crear(vertices, 4);
    rotar(rectangulo->vertices, 4, v[4]*PI/180);
    trasladar(rectangulo->vertices, 4, v[0], v[1]);

    printf("RECTANGULO: x = %d y = %d, ancho = %d, alto = %d, angulo = %d \n", v[0], v[1], v[2], v[3], v[4]);

    return rectangulo;
}


poligono_t *leer_geometria_poligono(FILE *f) {
    int16_t p;
    if(fread(&p, sizeof(int16_t), 1, f) != 1) {
      fprintf(stderr, "Error lectura\n");
      return NULL;
    }

    int16_t v[2];
    poligono_t *poligono = poligono_crear(NULL, 0);
    printf("POLIGONO: puntos = %d\n", p);

    for(size_t i = 0; i < p; i++) {
        if(fread(v, sizeof(int16_t), 2, f) != 2) {
             fprintf(stderr, "Error lectura\n");
             return NULL;
        }
    
      printf("\tx%zd = %d, y%zd = %d\n", i, v[0], i, v[1]);
      if(!poligono_agregar_vertice(poligono, v[0], v[1])) return NULL;
    }


    
    return poligono;
}


poligono_t *leer_geometria(FILE*f, geometria_t geometria) {
  poligono_t *poligono = geometrias[geometria](f);

  return poligono;
}




//// OBSTACULO ////


obstaculo_t *crear_obstaculo(geometria_t geometria, movimiento_t movimiento, color_t color, poligono_t *poligono, size_t n_parametros, int16_t *parametros) {
  obstaculo_t *obstaculo = malloc(sizeof(obstaculo_t)); 
  if(obstaculo == NULL)
    return NULL;
  
  obstaculo->n_parametros = n_parametros;
  
  for(size_t i = 0; i < n_parametros; i++)
    obstaculo->parametros[i] = parametros[i];
 

  obstaculo->geometria = geometria;
  obstaculo->movimiento = movimiento;
  obstaculo->color = color;
  obstaculo->poligono = poligono;

  obstaculo->impactos = 0;
  return obstaculo;
}

obstaculo_t *crear_atrapabolas(void) {
    float vertices[][2] = {{180,0}, {180,-30}, {165,-30}, {165,-15}, {15,-15}, {15,-30}, {0,-30}, {0,0}};
    int16_t parametros[3] = {MAX_X+100, 200, -376};
    //rotar(vertices, 8, PI);
    trasladar(vertices, 8, MIN_X, MAX_Y+15);
    obstaculo_t *atrapabolas = crear_obstaculo(GEO_POLIGONO, MOV_HORIZONTAL, COLOR_GRIS, poligono_crear(vertices, 8), 3, parametros);
    return atrapabolas;
}

void destruir_obstaculo(obstaculo_t *obstaculo) {
    puts("ENTRE AL DESTRUIR");
  poligono_destruir(obstaculo->poligono);
    puts("DESTRUI EL POLIGONO");
  free(obstaculo);
}


bool consultar_impacto(obstaculo_t *obstaculo) {
  return obstaculo->impactos;
}

void obstaculo_impactar(obstaculo_t *obstaculo) {
  obstaculo->impactos ++;
}

/*
obstaculo_t *obstaculo_crear_desde_archivo(FILE *f) {
  geometria_t geometria;
  movimiento_t movimiento;
  color_t color;

  leer_encabezado(f, &color, &movimiento, &geometria);

  poligono_t *poligono = leer_geometria(f, geometria);
  obstaculo_t *obstaculo = crear_obstaculo(geometria, movimiento, color, poligono);
  return obstaculo;
}
*/

#include <SDL2/SDL.h>
#include <stdbool.h>
#define DT (1.0 / JUEGO_FPS)

#ifdef TTF
#include <SDL2/SDL_ttf.h>

void mover(obstaculo_t *obstaculo) {

    switch(obstaculo->movimiento){
        case(MOV_INMOVIL): return;

        case(MOV_CIRCULAR): {
            rotar_eje(obstaculo->poligono->vertices, obstaculo->poligono->n, obstaculo->parametros[2]*DT, obstaculo->parametros[0], obstaculo->parametros[1]);
            return;
        }

        case(MOV_HORIZONTAL): {

            if(obstaculo->parametros[1] <= 0 || obstaculo->parametros[1] > obstaculo->parametros[0]) {
                obstaculo->parametros[2] *= -1;
            }

            trasladar(obstaculo->poligono->vertices, obstaculo->poligono->n, obstaculo->parametros[2]*DT, 0);
            obstaculo->parametros[1] += (obstaculo->parametros[2]*DT);
            return;
        }
    }
}



void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo) {
  switch(obstaculo->color) {
    case(COLOR_AZUL): {
              SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
              break;
            }
    case(COLOR_NARANJA):{
              SDL_SetRenderDrawColor(renderer, 0xFF, 0x80, 0x00, 0x00);
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
    case(COLOR_AMARILLO): {
              SDL_SetRenderDrawColor(renderer, 0xFF, 0xE9, 0x00, 0x00);
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




void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255, 255, 255};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

#endif


void lectura(FILE *f, SDL_Renderer *renderer, lista_t *lista) {

    int nivel = 0;
    while(1) {
        nivel++;

        int16_t obstaculos;
        if(!fread(&obstaculos, sizeof(int16_t), 1, f)) break;

        printf("Nivel %d, Cantidad de obstaculos: %u\n", nivel, obstaculos);

        

        for(size_t obstaculo = 0; obstaculo < obstaculos; obstaculo++) {
            color_t color;
            movimiento_t movimiento;
            geometria_t geometria;

            assert(leer_encabezado(f, &color, &movimiento, &geometria));



            

            printf("Obstaculo %zd, color = %s\n", obstaculo, colores[color]);

            int16_t parametros[4];
            size_t n_parametros;
            
            
            assert(leer_movimiento(f, movimiento, parametros, &n_parametros));

            printf("Movimiento: Parametros = %zd", n_parametros);
            for(size_t i = 0; i < n_parametros; i++)
                printf(", %d", parametros[i]);
            putchar('\n');

            lista_insertar_primero(lista, crear_obstaculo(geometria, movimiento, color, leer_geometria(f, geometria), n_parametros, parametros)); 
         
        }

        printf("Cant Obstaculos %d\n", obstaculos);
        printf("Largo de la lista %zd\n", lista_largo(lista));
        puts("HASTA ACA LLEGUE");
        return;
    }
    return;
  }


/////// CHOQUES

bool choque(obstaculo_t *obstaculo, float *cx, float *cy, float *vx, float *vy, size_t *poder) {
    float norm_x, norm_y;
    if(poligono_distancia(obstaculo->poligono, *cx, *cy, &norm_x, &norm_y) < BOLA_RADIO){
        obstaculo->impactos ++;
        reflejar(norm_x, norm_y, cx, cy, vx, vy);
        *vx *= PLASTICIDAD;
        *vy *= PLASTICIDAD;
        if(obstaculo->color == COLOR_VERDE) {
                    *poder = 1;
                    
                }
        if(obstaculo->color != COLOR_GRIS) obstaculo->color = COLOR_AMARILLO;
        return true;
    }
    return false;
}

void activar_poder(float *cx, float *cy, lista_t *lista) {
    lista_iter_t *iter = lista_iter_crear(lista);
    float norm_x, norm_y;
    ;
    while(!lista_iter_al_final(iter)) {
          obstaculo_t *aux = lista_iter_ver_actual(iter);

          if(100 > poligono_distancia(aux->poligono, *cx, *cy, &norm_x, &norm_y) && aux->color != COLOR_GRIS) {
            
            aux->color = COLOR_AMARILLO;
            lista_iter_avanzar(iter);
            continue;
            
          }
          
          lista_iter_avanzar(iter);
        }

        

}

void verificar_choques(lista_t *lista, size_t *naranja) {
    lista_iter_t *iter = lista_iter_crear(lista);
                        
        while(!lista_iter_al_final(iter)) {
            obstaculo_t *aux = lista_iter_ver_actual(iter);
            if(aux->color == COLOR_AMARILLO){
                    destruir_obstaculo(lista_iter_ver_actual(iter));
                    lista_iter_borrar(iter);
                        continue;
            }
                            
            if(aux->color == COLOR_NARANJA) {
                *naranja += 1;
            }
                lista_iter_avanzar(iter);
        }
                        
        lista_iter_destruir(iter);
        return;
}

//// FIN DE CHOQUES

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Error, dame un nivel\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No pudo abrirse \"%s\"\n", argv[1]);
        return 1;
    }


    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Peggle");

    for(size_t nivel = 1; nivel <= CANT_NIVELES; nivel++){

        lista_t *lista = lista_crear();
        lectura(f, renderer, lista);

        obstaculo_t *atrapabolas = crear_atrapabolas();
        lista_insertar_ultimo(lista, atrapabolas);
    
        int dormir = 0;
        
        // BEGIN código del alumno
        char s[30];
        sprintf(s, "Nivel %zd", nivel);
         
        int bolas = 13;
        bool bola_atrapada = false;
        size_t naranja = 0;    
        size_t poder = 0;
        float canon_angulo = 0; // Ángulo del cañón
        bool cayendo = false;   // ¿Hay bola disparada?
    
        float cx, cy;   // Centro de la bola
        float vx, vy;   // Velocidad de la bola
        // END código del alumno
    
        unsigned int ticks = SDL_GetTicks();
        while(1) {
            if(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    break;
    
                // BEGIN código del alumno
                if(event.type == SDL_MOUSEBUTTONDOWN) {
                    if(! cayendo)
                        cayendo = true;
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    canon_angulo = atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y);
                    if(canon_angulo > CANON_MAX)
                        canon_angulo = CANON_MAX;
                    if(canon_angulo < -CANON_MAX)
                        canon_angulo = -CANON_MAX;
                }
                // END código del alumno
    
                continue;
            }
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
    
    
            // BEGIN código del alumno
            #ifdef TTF
                
            if(bola_atrapada == true) escribir_texto(renderer, font, "Atrapaste la bolita", 450, 20);
            
            escribir_texto(renderer, font, "Peggle", 100, 20);
            escribir_texto(renderer, font, s , 350, 20);
            

            #endif
    
            if(poder) {
                        activar_poder(&cx, &cy, lista);
                        poder = 0;
                      }
    
            lista_iter_t *iter = lista_iter_crear(lista);
    
            while(!lista_iter_al_final(iter)) {
              obstaculo_t *aux = lista_iter_ver_actual(iter);
    
              
              mover(aux);
              choque(aux, &cx, &cy, &vx, &vy, &poder);

              if(aux->impactos >= 30 && aux->color != COLOR_GRIS){
                lista_iter_borrar(iter);
                lista_iter_avanzar(iter);
                continue;
              }
              dibujar_obstaculo(renderer, aux);
              lista_iter_avanzar(iter);
              
              
            }
    
    
            if(lista_iter_al_final(iter)){
                      lista_iter_destruir(iter);
            }
    
    
            if(cayendo) {
                // Si la bola está cayendo actualizamos su posición
                vy = computar_velocidad(vy, G, DT);
                vx *= ROZAMIENTO;
                vy *= ROZAMIENTO;
                cx = computar_posicion(cx, vx, DT);
                cy = computar_posicion(cy, vy, DT);
            }
            else {
                // Si la bola no se disparó establecemos condiciones iniciales
                cx = CANON_X + CANON_LARGO * sin(canon_angulo);
                cy = CANON_Y + CANON_LARGO * cos(canon_angulo);
                vx = BOLA_VI * sin(canon_angulo);
                vy = BOLA_VI * cos(canon_angulo);
            }
    
            // Rebote contra las paredes:
            if(cx < MIN_X + BOLA_RADIO || cx > MAX_X - BOLA_RADIO) vx = - vx;
            if(cy < MIN_Y + BOLA_RADIO) vy = -vy;
    
            // Se salió de la pantalla:
            if(cy > MAX_Y - BOLA_RADIO) {
                        if(atrapabolas->poligono->vertices[3][0] + BOLA_RADIO > cx && cx > atrapabolas->poligono->vertices[5][0] - BOLA_RADIO) {
                            bola_atrapada = true;
                            bolas ++;
                        }
                        else bola_atrapada = false;
                        cayendo = false;
    
                        
                        poder = 0;
                        bolas --;
                        
                        naranja = 0;
                        verificar_choques(lista, &naranja);
                        printf("Largo de la lista = %zd\n", lista->largo);
                        printf("Naranjas = %zd\n", naranja);
                        
                        printf("Naranjas = %zd\n", naranja);
                        if(naranja == 0) {
                            printf("Ganaste\n");
                            //lista_destruir(lista, destruir_obstaculo);
                            break;
                        }

                        if(bolas < 0){
                            printf("Perdiste, perdiste, no hay nadie peor que vos\n");
                            //bolas ++;
                            //lista_destruir(lista, destruir_obstaculo);  
                            nivel = CANT_NIVELES;
                            break;
                            
                        }
    
                        
    
    
            }
    
            // Dibujo cantidad de bolas
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            for(size_t i = 0; i < bolas; i++) {
                dibujar_circulo(renderer, circular(RESOLUCION, 15, 40, 40*(i+1)));
            }
    
            // Dibujamos el cañón:
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);
    
            // Dibujamos la bola:
            poligono_t *bola = circular(RESOLUCION, BOLA_RADIO, cx, cy);
            dibujar_circulo(renderer, bola);
    
            // Dibujamos las paredes:
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
            SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
            SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
            SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
            SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);
    
            // Dibujamos el vector de velocidad:
            float x_vector = cx;
            float y_vector = cy;
            float vel_vector_y = vy;
            float vel_vector_x = vx;
            while(y_vector < MAX_Y) {
                SDL_RenderDrawLine(renderer, x_vector, y_vector, x_vector + vel_vector_x * DT, y_vector + vel_vector_y * DT);
                x_vector = computar_posicion(x_vector, vel_vector_x, DT);
                y_vector = computar_posicion(y_vector, vel_vector_y, DT);
                vel_vector_y = computar_velocidad(vel_vector_y, G, DT);
            }
    
            //SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy + vy);
            // END código del alumno
    
            SDL_RenderPresent(renderer);
            ticks = SDL_GetTicks() - ticks;
            if(dormir) {
                SDL_Delay(dormir);
                dormir = 0;
            }
            else if(ticks < 1000 / JUEGO_FPS)
                SDL_Delay(1000 / JUEGO_FPS - ticks);
            ticks = SDL_GetTicks();
        
    
    
    
    
    
        }
    }


    // BEGIN código del alumno
    printf("Cierro el programa\n");
    fclose(f);


    // END código del alumno
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}

