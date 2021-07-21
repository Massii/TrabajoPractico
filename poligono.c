#include "poligono.h"
#include "operaciones.h"

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


poligono_t *circular(size_t resolucion, size_t radio, size_t x, size_t y) {
    float vertices[resolucion][2];

    float angulo = 360/resolucion;

    size_t j = resolucion-1;
    for(size_t i = 0; i < resolucion; i++, j--) {
      vertices[j][0] = radio*cos(i*angulo*PI/180);
      vertices[j][1] = radio*sin(i*angulo*PI/180);
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