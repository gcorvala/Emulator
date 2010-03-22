CC = gcc -Wall -pedantic -O4 -Werror -std=gnu99 -Wextra

OBJECTS = \
	cpu_gb.o \
	display_gb.o \
	map_gb.o \
	pixel_gb.o \
	rom_gb.o \
	sprite_gb.o \
	tile_gb.o

#cpu_nes.o
#map_nes.o
#rom_nes.o

BIN = main

%.o: %.c %.h
	$(CC) $< -c -o $@

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)

memcheck: main
	valgrind --leak-check=full ./main
