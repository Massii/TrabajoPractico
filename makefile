PROGRAM=ex_SDL
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g
LDFLAGS=-lm -lSDL2 -lSDL2_ttf

all: $(PROGRAM)

$(PROGRAM): test_sdl2.o operaciones.o
	$(CC) $(CFLAGS) $(LDFLAGS) test_sdl2.o operaciones.o -o $(PROGRAM)

test_sdl2.o: config.h operaciones.c operaciones.h
	$(CC) $(CFLAGS) -c test_sdl2.c

velocidad.o: config.h operaciones.h
	$(CC) $(CFLAGS) -c operaciones.c

clean:
	rm -vf *.o $(PROGRAM)
