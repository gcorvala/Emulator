#ifndef _MAPPER_H_
#define _MAPPER_H_

#include "cpu.h"
#include "rom_nes.h"
#include "ppu.h"
#include "papu.h"
#include "types.h"

typedef struct {
  CPU *cpu;
  RomNES *rom;
  PPU *ppu;
  PAPU *papu;
  BYTE nmi[2];
  BYTE reset[2];
  BYTE irq[2];
} Mapper;

Mapper * mapper_new (CPU *cpu, RomNES *rom, PPU *ppu, PAPU *papu);
BYTE     mapper_get_byte_at_addr (Mapper *mapper, BYTE addr);
void     mapper_set_byte_at_addr (Mapper *mapper, BYTE addr);

#endif