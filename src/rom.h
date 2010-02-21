#ifndef _ROM_H_
#define _ROM_H_

#include "types.h"

#include <stddef.h>

typedef struct _Rom Rom;

Rom *               rom_new             (const char *filename);
void                rom_free            (Rom *rom);
BYTE                rom_get_prg_memory  (Rom *rom, ADDR addr);
BYTE                rom_get_prg_n_pages (Rom *rom);
size_t              rom_get_prg_size    (Rom *rom);
BYTE                rom_get_chr_memory  (Rom *rom, ADDR addr);
BYTE                rom_get_chr_n_pages (Rom *rom);
size_t              rom_get_chr_size    (Rom *rom);
//int                 rom_is_v_mirroring  (Rom *rom);
//int                 rom_is_h_mirroring  (Rom *rom);
//int                 rom_is_sram         (Rom *rom);
//int                 rom_is_trainer      (Rom *rom);
//int                 rom_is_four_screen  (Rom *rom);
short unsigned int  rom_get_mapper_id   (Rom *rom);

#endif