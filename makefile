PROGRAM=ex_SDL
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g
LDFLAGS=-lm -lSDL2 -lSDL2_ttf

all: $(PROGRAM)

$(PROGRAM): main.o lista.o operaciones.o lectura.o poligono.o obstaculo.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o lista.o operaciones.o lectura.o poligono.o obstaculo.o -o $(PROGRAM)

main.o: main.h obstaculo.h operaciones.h config.h lectura.h lista.h
	$(CC) $(CFLAGS) -c main.c

lista.o: lista.h
	$(CC) $(CFLAGS) -c lista.c

operaciones.o: operaciones.h
	$(CC) $(CFLAGS) -c operaciones.c

lectura.o: lectura.h poligono.h operaciones.h config.h
	$(CC) $(CFLAGS) -c lectura.c

poligono.o: poligono.h operaciones.h config.h
	$(CC) $(CFLAGS) -c poligono.c

obstaculo.o: obstaculo.h poligono.h lectura.h config.h
	$(CC) $(CFLAGS) -c obstaculo.c
clean:
	rm -vf *.o $(PROGRAM)
