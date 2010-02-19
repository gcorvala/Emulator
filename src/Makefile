CC = gcc -Wall

OBJECTS = cpu.o \
	mapper.o \
	papu.o \
	ppu.o \
	rom_nes.o

BIN = main

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)
