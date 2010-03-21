CC = gcc -Wall -pedantic -O4 -Werror -std=gnu99

OBJECTS = \
	cpu_gb.o \
	cpu_nes.o \
	display_gb.o \
	map_gb.o \
	map_nes.o \
	rom_gb.o \
	rom_nes.o \
	tile_gb.o

BIN = main

%.o: %.c %.h
	$(CC) $< -c -o $@

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)

memcheck: main
	valgrind --leak-check=full ./main
