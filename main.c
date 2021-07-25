#include <SDL2/SDL.h>
//#define DT (1.0 / JUEGO_FPS)

#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "main.h"
#include "config.h"
#include "obstaculo.h"
#include "operaciones.h"
#include "lectura.h"
#include "lista.h"
#include "choques.h"

#ifdef TTF
#include <SDL2/SDL_ttf.h>


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

/*
void lectura(int argc, char *argv[], SDL_Renderer *renderer, lista_t *lista) {

    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No pudo abrirse \"%s\"\n", argv[1]);
        return;
    }
*/
  
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

    for(size_t nivel = 1; nivel < CANT_NIVELES; nivel++){
        int dormir = 0;

        // BEGIN código del alumno
        double puntos = 0;
        double contador_naranjas = 0;
        double multiplicador = 1;

        lista_t *lista = lista_crear();
        lectura(f, renderer, lista);
        
        obstaculo_t *atrapabolas = crear_atrapabolas();
        lista_insertar_ultimo(lista, atrapabolas);
        
        char s_nivel[20];
        sprintf(s_nivel, "Nivel %zd", nivel);

        char s_puntos[50];
        sprintf(s_puntos, "Puntos %.1f", puntos*multiplicador);
    
        int bolas = MAX_BOLITAS;

        poligono_t *bolas_poligonos[MAX_BOLITAS];
        
        for(size_t i = 0; i < MAX_BOLITAS; i++) {
            bolas_poligonos[i] = circular(RESOLUCION, 15, 40, 40*(i+1));
        }

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
                if(!cayendo)
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
        if(bola_atrapada == true) escribir_texto(renderer, font, "Atrapaste la bolita", 570, 20);

        escribir_texto(renderer, font, "Peggle", 100, 20);
        escribir_texto(renderer, font, s_nivel , 250, 20);
        #endif

        if(poder) {
            activar_poder(&cx, &cy, lista, &puntos, &contador_naranjas);
            poder = 0;
        }

        lista_iter_t *iter = lista_iter_crear(lista);

        while(!lista_iter_al_final(iter)) {
            obstaculo_t *aux = lista_iter_ver_actual(iter);
            mover(aux);
            choque(aux, &cx, &cy, &vx, &vy, &poder, &puntos, &contador_naranjas);

            if(aux->impactos >= 30 && aux->color != COLOR_GRIS){
                destruir_obstaculo(lista_iter_ver_actual(iter));
                lista_iter_borrar(iter);
                lista_iter_avanzar(iter);
                continue;
            }

            dibujar_obstaculo(renderer, aux);
            lista_iter_avanzar(iter);
        }


        /*for(size_t i = 0; i < lista->largo; i++) {
          dibujar_obstaculo(renderer, lista_iter_ver_actual(iter));
          lista_iter_avanzar(iter);
        }*/
            /*
        for(size_t i = 0; i < lista->largo; i++) {
          obstaculo_t *aux = lista_iter_ver_actual(iter);

          dibujar_obstaculo(renderer, aux);
          mover(aux);
          lista_iter_insertar(iter, aux);
          lista_iter_borrar(iter);
          lista_iter_avanzar(iter);
        }
        */


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

            if(contador_naranjas > 10) multiplicador = 2;
            if(contador_naranjas > 15) multiplicador = 3;
            if(contador_naranjas > 19) multiplicador = 5;
            if(contador_naranjas > 21) multiplicador = 10;
            sprintf(s_puntos, "Puntos %.1f   X%.0f", puntos*multiplicador, multiplicador);



            poder = 0;
            bolas --;

            naranja = 0;
            verificar_choques(lista, &naranja);
            printf("Largo de la lista = %zd\n", lista->largo);
            
            printf("Naranjas = %zd\n", naranja);
            //printf("Naranjas = %zd\n", naranja);

            if(naranja == 0) {
                printf("Ganaste\n");
                //Libero lo que quedó // también podría ser lista_destruir
                lista_iter_t *iter = lista_iter_crear(lista);
                while(!lista_iter_al_final(iter)) {
                    destruir_obstaculo(lista_iter_ver_actual(iter));
                    lista_iter_borrar(iter);
                }
                lista_iter_destruir(iter);
                free(lista);
                break;
            }

            if(bolas < 0){
                printf("Perdiste, perdiste, no hay nadie peor que vos\n");
                //Libero lo que quedó // también podría ser lista_destruir
                lista_iter_t *iter = lista_iter_crear(lista);
                while(!lista_iter_al_final(iter)) {
                    destruir_obstaculo(lista_iter_ver_actual(iter));
                    lista_iter_borrar(iter);
                }
                lista_iter_destruir(iter);
                free(lista);
                nivel = CANT_NIVELES;
                break;
            }
        }

        //-
        // Dibujo cantidad de bolas 
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        for(size_t i = 0; i < bolas; i++) {
            dibujar_circulo(renderer, bolas_poligonos[i]);
        }

        // Dibujamos el cañón:
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);

        // Dibujamos la bola:
        poligono_t *bola = circular(RESOLUCION, BOLA_RADIO, cx, cy);
        dibujar_circulo(renderer, bola);
        poligono_destruir(bola);

        // Dibujamos las paredes:
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
        SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
        SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
        SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
        SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

        /*// Dibujamos el vector de velocidad:
        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy + vy);
        */
        //-
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

        // Escribo Puntos
        escribir_texto(renderer, font, s_puntos , 340, 20);
        
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

    //Libero memoria de contador de pelotitas
    for(size_t i = 0; i < MAX_BOLITAS; i++){
        poligono_destruir((bolas_poligonos[i]));
    }

    //En caso de que no se haya borrado bien la lista (si cerras la ventana y naranjas != 0 y bolas > 0)
    if(!lista_esta_vacia(lista)){
        lista_iter_t *iter = lista_iter_crear(lista);
        while(!lista_iter_al_final(iter)) {
            destruir_obstaculo(lista_iter_ver_actual(iter));
            lista_iter_borrar(iter);
        }

        lista_iter_destruir(iter);
        free(lista);
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