CC = gcc -Wall

OBJECTS = cpu_2A03.o \
	mapper.o \
	papu.o \
	ppu.o \
	rom_nes.o

BIN = main

all = $(OBJECTS)

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)
