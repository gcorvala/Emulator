#include "mapper.h"

#include <stdlib.h>
#include <stdio.h>

Mapper *
mapper_new (RomNES *rom) {
  Mapper *mapper;

  mapper = calloc (sizeof (Mapper), 1);

  mapper->rom = rom;

  return mapper;
}

BYTE
mapper_get_memory (Mapper *mapper, ADDR addr) {
  BYTE value = 0;

  if (addr < 0x8000) {
    printf ("\tmapper_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr >= 0x8000) {
    ADDR real_addr;

    real_addr = addr % 0x8000;
    printf ("mapper_get_memory at %04x is a real_address %p ", addr, mapper->rom->prg + real_addr);
    value = *(mapper->rom->prg + real_addr);
  }

  return value;
}
