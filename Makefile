PROGRAM=Salida
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g
LDFLAGS=-lSDL2 -lSDL2_ttf -DTTF -lm

all: $(PROGRAM)

$(PROGRAM): sdl2.o operaciones.o poligono.o
	$(CC) -o $(PROGRAM) sdl2.o operaciones.o poligono.o obstaculo.o lectura.o  $(LDFLAGS)

sdl2.o: 
	$(CC) $(CFLAGS) -c sdl2.c $(LDFLAGS) 

obstaculo.o: 
	$(CC) $(CFLAGS) -c obstaculo.c

lectura.o: 
	$(CC) $(CFLAGS) -c lectura.c

poligono.o: 
	$(CC) $(CFLAGS) -c poligono.c

operaciones.o: 
	$(CC) $(CFLAGS) -c operaciones.c

clean:
	rm -vf *.o $(PROGRAM)

