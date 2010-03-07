#ifndef _ROM_GB_H_
#define _ROM_GB_H_

#include "types.h"

#include <stddef.h>

typedef struct _RomGB RomGB;

RomGB * rom_gb_new              (const char *filename);
void    rom_gb_free             (RomGB *rom);
BOOL    rom_gb_check_logo       (RomGB *rom);
char *  rom_gb_get_title        (RomGB *rom);
char *  rom_gb_get_manufacturer (RomGB *rom);
BYTE    rom_gb_get_color_flag   (RomGB *rom);
char *  rom_gb_get_new_licence  (RomGB *rom);
BYTE    rom_gb_get_super_flag   (RomGB *rom);
BYTE    rom_gb_get_type         (RomGB *rom);
size_t  rom_gb_get_rom_size     (RomGB *rom);
size_t  rom_gb_get_ram_size     (RomGB *rom);
BYTE    rom_gb_get_destination  (RomGB *rom);
BYTE    rom_gb_get_old_licence  (RomGB *rom);
BYTE    rom_gb_get_version      (RomGB *rom);
BOOL    rom_gb_check_header     (RomGB *rom);
BOOL    rom_gb_check_full       (RomGB *rom);

#endif