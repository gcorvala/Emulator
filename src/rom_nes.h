#ifndef _ROM_NES_H_
#define _ROM_NES_H_

#include "types.h"

typedef struct {
  BYTE prg_page_count;
  BYTE chr_page_count;
  BYTE *prg;
  BYTE *chr;
  int mirroring : 1;
  int sram : 1;
  int trainer : 1;
  int four_screen : 1;
} RomNES;

RomNES * rom_nes_new (const char *filename);
void     rom_nes_free (RomNES *rom);

#endif