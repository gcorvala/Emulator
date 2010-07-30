CC = gcc -pedantic -O4 -std=gnu99 -Wall -Wextra -Werror
LIBS = sdl

OBJECTS = \
	background_gb.o \
  color.o \
  cpu.o \
	cpu_gb.o \
	cpu_op_16b.o \
	map_gb.o \
	rom_gb.o \
	tile_gb.o \
	utils.o

HEADERS = \
	background_gb.h.gch \
	color.h.gch \
	cpu.h.gch \
	cpu_gb.h.gch \
	cpu_op_16b.h.gch \
	map_gb.h.gch \
	rom_gb.h.gch \
	tile_gb.h.gch \
	types.h.gch \
	utils.h.gch

TESTS = \
	test_cpu_ops.o

BIN = main test

%.o: %.c %.h
	$(CC) $< -c -o $@

%.h.gch: %.h
	$(CC) $< -o $@

main: main.c $(HEADERS) $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@ `pkg-config --cflags --libs $(LIBS)`

test: $(TESTS) $(HEADERS) $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(HEADERS) $(OBJECTS) $(BIN) $(TESTS)

memcheck: main
	valgrind --leak-check=full ./main
