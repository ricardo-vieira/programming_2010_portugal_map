CC=gcc
CFLAGS=-c -g -ansi -pedantic -Wall
LDFLAGS= -lm `allegro-config --libs`
SOURCES=main.c carrega.c gestao.c mapas.c rotas.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=prog1011

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $<
	
clean:
	rm -rf *.o prog1011 *g.scd
open:
	gedit $(SOURCES) gps.h &
