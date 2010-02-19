#include "mapper.h"

#include <stdlib.h>

Mapper *
mapper_new (CPU2A03 *cpu, RomNES *rom, PPU *ppu, PAPU *papu) {
  Mapper *mapper;

  mapper = calloc (sizeof (Mapper), 1);

  mapper->cpu = cpu;
  mapper->rom = rom;
  mapper->ppu = ppu;
  mapper->papu = papu;

  return mapper;
}

BYTE
mapper_get_byte_at_addr (Mapper *mapper, BYTE addr) {
  BYTE result = 0;

  return result;
}
