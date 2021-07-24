PROGRAM=ex_SDL
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g
LDFLAGS=-lm -lSDL2 -lSDL2_ttf -DTTF

all: $(PROGRAM)

$(PROGRAM): main.o lista.o operaciones.o lectura.o poligono.o obstaculo.o choques.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o lista.o operaciones.o lectura.o poligono.o obstaculo.o choques.o -o $(PROGRAM)

main.o: main.h obstaculo.h operaciones.h config.h lectura.h lista.h choques.h
	$(CC) $(CFLAGS) -c main.c

lista.o: lista.h
	$(CC) $(CFLAGS) -c lista.c

operaciones.o: operaciones.h
	$(CC) $(CFLAGS) -c operaciones.c

lectura.o: lectura.h poligono.h operaciones.h config.h
	$(CC) $(CFLAGS) -c lectura.c

poligono.o: poligono.h operaciones.h config.h
	$(CC) $(CFLAGS) -c poligono.c

obstaculo.o: obstaculo.h poligono.h lectura.h config.h operaciones.h main.h
	$(CC) $(CFLAGS) -c obstaculo.c

choques.o: choques.h obstaculo.h poligono.h lectura.h config.h operaciones.h lista.h
	$(CC) $(CFLAGS) -c choques.c

clean:
	rm -vf *.o $(PROGRAM)
