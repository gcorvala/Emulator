CC = gcc -pedantic -O4 -std=gnu99 -Wall -Wextra -Werror
LIBS = sdl

OBJECTS = \
	background_gb.o \
  color.o \
	cpu_gb.o \
	map_gb.o \
	rom_gb.o \
	tile_gb.o

HEADERS = \
	background_gb.h.gch \
	color.h.gch \
	cpu_gb.h.gch \
	map_gb.h.gch \
	rom_gb.h.gch \
	tile_gb.h.gch \
	types.h.gch

BIN = main

%.o: %.c %.h
	$(CC) $< -c -o $@

%.h.gch: %.h
	$(CC) $< -o $@

main: main.c $(HEADERS) $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@ `pkg-config --cflags --libs $(LIBS)`

clean:
	rm -f $(HEADERS) $(OBJECTS) $(BIN)

memcheck: main
	valgrind --leak-check=full ./main
