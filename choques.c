#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <math.h>


#include "choques.h"
/*
#include "obstaculo.h"
#include "lectura.h"
#include "lista.h"
#include "poligono.h"
#include "config.h"
#include "operaciones.h"
*/

bool choque(obstaculo_t *obstaculo, float *cx, float *cy, float *vx, float *vy, size_t *poder, double *puntos, double *contador_naranjas) {
    float norm_x = 0, norm_y = 0;
    if(poligono_distancia(obstaculo->poligono, *cx, *cy, &norm_x, &norm_y) < BOLA_RADIO){
        obstaculo->impactos ++;
        reflejar(norm_x, norm_y, cx, cy, vx, vy);
        *vx *= PLASTICIDAD;
        *vy *= PLASTICIDAD;
        if(obstaculo->color == COLOR_VERDE) {
                    *poder = 1;
                    *puntos += 10;
                    obstaculo->color = COLOR_AMARILLO;
        }
        
        if(obstaculo->color == COLOR_AZUL) {
            *puntos += 10;
            obstaculo->color = COLOR_AMARILLO;
        }

        if(obstaculo->color == COLOR_NARANJA) {
            *puntos += 100;
            *contador_naranjas += 1;
            obstaculo->color = COLOR_AMARILLO;
        }


        return true;
    }
    return false;
}


void activar_poder(float *cx, float *cy, lista_t *lista, double *puntos, double *contador_naranjas) {
    lista_iter_t *iter = lista_iter_crear(lista);
    float norm_x, norm_y;
    ;
    while(!lista_iter_al_final(iter)) {
          obstaculo_t *aux = lista_iter_ver_actual(iter);

          if(100 > poligono_distancia(aux->poligono, *cx, *cy, &norm_x, &norm_y) && aux->color != COLOR_GRIS) {
            if(aux->color == COLOR_VERDE) {
                    *puntos += 10;
                    aux->color = COLOR_AMARILLO;
            }
        
            if(aux->color == COLOR_AZUL) {
                *puntos += 10;
                aux->color = COLOR_AMARILLO;
            }

            if(aux->color == COLOR_NARANJA) {
                *puntos += 100;
                *contador_naranjas += 1;
                aux->color = COLOR_AMARILLO;
            }

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
