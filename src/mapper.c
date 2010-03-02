#include "mapper.h"

#include <stdlib.h>
#include <stdio.h>

Mapper *
mapper_new (Rom *rom) {
  Mapper *mapper;

  mapper = calloc (sizeof (Mapper), 1);

  mapper->rom = rom;

  return mapper;
}

void
mapper_free (Mapper *mapper) {
  free (mapper);
}


BYTE
mapper_get_memory (Mapper *mapper, ADDR16 addr) {
  BYTE value = 0;

  if (addr < 0x8000) {
    printf ("\tmapper_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr >= 0x8000) {
    ADDR16 real_addr;

    real_addr = addr % 0x8000;
    /* printf ("\tmapper_get_memory at %04x is a real_address %p\n", addr, mapper->rom->prg + real_addr); */
    value = rom_get_prg_memory (mapper->rom, real_addr);
  }

  return value;
}
