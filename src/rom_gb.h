#ifndef _ROM_GB_H_
#define _ROM_GB_H_

#include "types.h"

#include <stddef.h>

RomGB *   rom_gb_new              (const char *filename);
void      rom_gb_free             (RomGB *rom);
char *    rom_gb_get_title        (RomGB *rom);
UINT16    rom_gb_get_licence      (RomGB *rom);
UINT8     rom_gb_get_type         (RomGB *rom);
size_t    rom_gb_get_rom_size     (RomGB *rom);
size_t    rom_gb_get_ram_size     (RomGB *rom);
UINT8     rom_gb_get_version      (RomGB *rom);
BOOL      rom_gb_has_color_flag   (RomGB *rom);
BOOL      rom_gb_has_super_flag   (RomGB *rom);
BOOL      rom_gb_is_for_japan     (RomGB *rom);
BOOL      rom_gb_check_logo       (RomGB *rom);
BOOL      rom_gb_check_header     (RomGB *rom);
BOOL      rom_gb_check_full       (RomGB *rom);
BYTE      rom_gb_get_rom_memory   (RomGB *rom, UINT8 bank, ADDR16 addr);
BYTE      rom_gb_get_ram_memory   (RomGB *rom, UINT8 bank, ADDR16 addr);
void      rom_gb_set_ram_memory   (RomGB *rom, UINT8 bank, ADDR16 addr, BYTE value);

#endif
