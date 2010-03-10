CC = gcc -Wall -pedantic

OBJECTS = cpu_6502.o \
	cpu_gb.o \
	mapper.o \
	rom_gb.o \
	rom_nes.o

BIN = main

%.o: %.c %.h
	$(CC) $< -c -o $@

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)
