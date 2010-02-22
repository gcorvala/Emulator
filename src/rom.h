#ifndef _ROM_H_
#define _ROM_H_

#include "types.h"

#include <stddef.h>

typedef struct _Rom Rom;

Rom *               rom_new                  (const char *filename);
void                rom_free                 (Rom *rom);
BYTE                rom_get_prg_memory       (Rom *rom, ADDR16 addr);
unsigned int        rom_get_prg_n_pages      (Rom *rom);
size_t              rom_get_prg_size         (Rom *rom);
BYTE                rom_get_chr_memory       (Rom *rom, ADDR16 addr);
unsigned int        rom_get_chr_n_pages      (Rom *rom);
size_t              rom_get_chr_size         (Rom *rom);
int                 rom_get_mirroring_flag   (Rom *rom);
int                 rom_get_sram_flag        (Rom *rom);
int                 rom_get_trainer_flag     (Rom *rom);
int                 rom_get_four_screen_flag (Rom *rom);
unsigned short int  rom_get_mapper_id        (Rom *rom);

#endif