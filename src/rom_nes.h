#ifndef _ROM_NES_H_
#define _ROM_NES_H_

#include "types.h"

#include <stddef.h>

RomNES *            rom_nes_new                  (const char *filename);
void                rom_nes_free                 (RomNES *rom);
BYTE                rom_nes_get_prg_memory       (RomNES *rom, ADDR16 addr);
unsigned int        rom_nes_get_prg_n_pages      (RomNES *rom);
size_t              rom_nes_get_prg_size         (RomNES *rom);
BYTE                rom_nes_get_chr_memory       (RomNES *rom, ADDR16 addr);
unsigned int        rom_nes_get_chr_n_pages      (RomNES *rom);
size_t              rom_nes_get_chr_size         (RomNES *rom);
int                 rom_nes_get_mirroring_flag   (RomNES *rom);
int                 rom_nes_get_sram_flag        (RomNES *rom);
int                 rom_nes_get_trainer_flag     (RomNES *rom);
int                 rom_nes_get_four_screen_flag (RomNES *rom);
unsigned short int  rom_nes_get_mapper_id        (RomNES *rom);

#endif