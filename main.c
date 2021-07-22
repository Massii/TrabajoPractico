#include <SDL2/SDL.h>
#include <stdbool.h>
#define DT (1.0 / JUEGO_FPS)

#ifdef TTF
#include <SDL2/SDL_ttf.h>


void dibujar_obstaculo(SDL_Renderer *renderer, obstaculo_t *obstaculo) {
  switch(obstaculo->color) {
      case(COLOR_AZUL): {
              SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
              break;
            }
      case(COLOR_NARANJA):{
              SDL_SetRenderDrawColor(renderer, 0xFF, 0xC0, 0x00, 0x00);
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

void lectura(int argc, char *argv[], SDL_Renderer *renderer, lista_t *lista) {

    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No pudo abrirse \"%s\"\n", argv[1]);
        return;
    }

  

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

            lista_insertar_primero(lista, crear_obstaculo(geometria, movimiento, color, leer_geometria(f, geometria))); 

        }

        printf("Cant Obstaculos %zd\n", obstaculos);
        printf("Largo de la lista %zd\n", lista_largo(lista));
        puts("HASTA ACA LLEGUE");
        return;
    }
    return;
  }

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    lista_t *lista = lista_crear();

    lectura(argc, argv, renderer, lista);
    

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Peggle");

    int dormir = 0;
    // BEGIN código del alumno
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
        escribir_texto(renderer, font, "Peggle", 100, 20);
        #endif

        lista_iter_t *iter = lista_iter_crear(lista);

        for(size_t i = 0; i < lista->largo; i++) {
          dibujar_obstaculo(renderer, lista_iter_ver_actual(iter));
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
        if(cy > MAX_Y - BOLA_RADIO)
            cayendo = false;


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
        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy + vy);
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


    // BEGIN código del alumno
  



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

