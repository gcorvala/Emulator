CC = gcc -Wall -pedantic -O4 -Werror -std=gnu99

OBJECTS = \
	cpu_gb.o \
	map_gb.o \
	mapper.o \
	rom_gb.o \
	rom_nes.o
#cpu_6502.o \

BIN = main

%.o: %.c %.h
	$(CC) $< -c -o $@

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)
