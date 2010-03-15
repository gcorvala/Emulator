#ifndef _MAPPER_H_
#define _MAPPER_H_

#include "types.h"

MapNES * map_nes_new (RomNES *rom);
void     map_nes_free (MapNES *mapper);
BYTE     map_nes_get_memory (MapNES *mapper, ADDR16 addr);
void     map_nes_set_memory (MapNES *mapper, ADDR16 addr);

#endif