#include "map_nes.h"

#include "rom_nes.h"

#include <stdlib.h>
#include <stdio.h>

struct _MapNES {
  RomNES *rom;
  BYTE nmi[2];
  BYTE reset[2];
  BYTE irq[2];
};

MapNES *
map_nes_new (RomNES *rom) {
  MapNES *mapper;

  mapper = calloc (sizeof (MapNES), 1);

  mapper->rom = rom;

  return mapper;
}

void
map_nes_free (MapNES *mapper) {
  free (mapper);
}


BYTE
map_nes_get_memory (MapNES *mapper, ADDR16 addr) {
  BYTE value = 0;

  if (addr < 0x8000) {
    printf ("\tmap_nes_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr >= 0x8000) {
    ADDR16 real_addr;

    real_addr = addr % 0x8000;
    /* printf ("\tmap_nes_get_memory at %04x is a real_address %p\n", addr, mapper->rom->prg + real_addr); */
    value = rom_nes_get_prg_memory (mapper->rom, real_addr);
  }

  return value;
}
