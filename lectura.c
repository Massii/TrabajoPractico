#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "lectura.h"
#include "poligono.h"

bool (*movimientos[])(FILE *f, int16_t parametros[], size_t *n_parametros) = {
	[MOV_INMOVIL] = leer_movimiento_inmovil, 
	[MOV_CIRCULAR] = leer_movimiento_circular, 
	[MOV_HORIZONTAL] = leer_movimiento_horizontal
};



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
    float vertices[][2] = {{v[2]/2, v[3]/2}, {v[2]/2, -v[3]/2}, {-v[2]/2, -v[3]/2}, {-v[2]/2, v[3]/2}};
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


int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No pudo abrirse \"%s\"\n", argv[1]);
        return 1;
    }

  

    int nivel = 0;
    while(1) {
        nivel++;

        int16_t obstaculos;
        if(! fread(&obstaculos, sizeof(int16_t), 1, f)) break;

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

            assert(leer_geometria(f, geometria) == NULL);
        }
    }

    fclose(f);

    return 0;
}
