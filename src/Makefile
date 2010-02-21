CC = gcc -Wall

OBJECTS = cpu.o \
	mapper.o \
	papu.o \
	ppu.o \
	rom.o

BIN = main

%.o: %.c %.h
	$(CC) $< -c -o $@

main: main.c $(OBJECTS)
	$(CC) $< $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN)
