#ifndef _ROM_GB_H_
#define _ROM_GB_H_

#include "types.h"

#include <stddef.h>

typedef struct _RomGB RomGB;

typedef enum {
  ROM_GB_ROM_ONLY                = 0x00,
  ROM_GB_MBC1                    = 0x01,
  ROM_GB_MBC1_RAM                = 0x02,
  ROM_GB_MBC1_RAM_BATTERY        = 0x03,
  ROM_GB_MBC2                    = 0x05,
  ROM_GB_MBC2_BATTERY            = 0x06,
  ROM_GB_ROM_RAM                 = 0x08,
  ROM_GB_ROM_RAM_BATTERY         = 0x09,
  ROM_GB_MMM01                   = 0x0B,
  ROM_GB_MMM01_RAM               = 0x0C,
  ROM_GB_MMM01_RAM_BATTERY       = 0x0D,
  ROM_GB_MBC3_TIMER_BATTERY      = 0x0F,
  ROM_GB_MBC3_TIMER_RAM_BATTERY  = 0x10,
  ROM_GB_MBC3                    = 0x11,
  ROM_GB_MBC3_RAM                = 0x12,
  ROM_GB_MBC3_RAM_BATTERY        = 0x13,
  ROM_GB_MBC4                    = 0x15,
  ROM_GB_MBC4_RAM                = 0x16,
  ROM_GB_MBC4_RAM_BATTERY        = 0x17,
  ROM_GB_MBC5                    = 0x19,
  ROM_GB_MBC5_RAM                = 0x1A,
  ROM_GB_MBC5_RAM_BATTERY        = 0x1B,
  ROM_GB_MBC5_RUMBLE             = 0x1C,
  ROM_GB_MBC5_RUMBLE_RAM         = 0x1D,
  ROM_GB_MBC5_RUMBLE_RAM_BATTERY = 0x1E,
  ROM_GB_POCKET_CAMERA           = 0xFC,
  ROM_GB_BANDAI_TAMAS            = 0xFD,
  ROM_GB_HUC3                    = 0xFE,
  ROM_GB_HUC1_RAM_BATTERY        = 0xFF
} RomGBType;

RomGB *   rom_gb_new              (const char *filename);
void      rom_gb_free             (RomGB *rom);
char *    rom_gb_get_title        (RomGB *rom);
UINT16    rom_gb_get_licence      (RomGB *rom);
RomGBType rom_gb_get_type         (RomGB *rom);
size_t    rom_gb_get_rom_size     (RomGB *rom);
size_t    rom_gb_get_ram_size     (RomGB *rom);
UINT8     rom_gb_get_version      (RomGB *rom);
BOOL      rom_gb_has_color_flag   (RomGB *rom);
BOOL      rom_gb_has_super_flag   (RomGB *rom);
BOOL      rom_gb_is_for_japan     (RomGB *rom);
BOOL      rom_gb_check_logo       (RomGB *rom);
BOOL      rom_gb_check_header     (RomGB *rom);
BOOL      rom_gb_check_full       (RomGB *rom);

#endif
