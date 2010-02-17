CC = gcc

OBJECTS = cpu_6502.o \
	rom_nes.o

BIN = main

all = $(OBJECTS)

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)