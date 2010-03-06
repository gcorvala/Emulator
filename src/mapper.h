#ifndef _MAPPER_H_
#define _MAPPER_H_

#include "rom_nes.h"
#include "types.h"

typedef struct {
  RomNES *rom;
  BYTE nmi[2];
  BYTE reset[2];
  BYTE irq[2];
} Mapper;

Mapper * mapper_new (RomNES *rom);
void     mapper_free (Mapper *mapper);
BYTE     mapper_get_memory (Mapper *mapper, ADDR16 addr);
void     mapper_set_memory (Mapper *mapper, ADDR16 addr);

#endif