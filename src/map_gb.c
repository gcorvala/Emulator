#include "map_gb.h"

#include "cpu_gb.h"
#include "rom_gb.h"
#include <stdlib.h>
#include <stdio.h>

struct _MapGB {
  CpuGB *cpu;
  RomGB *rom;
  BYTE tmp[0x80];
};

MapGB *
map_gb_new (void) {
  MapGB *map;

  map = calloc (sizeof (MapGB), 1);

  return map;
}

void
map_gb_free (MapGB *map) {
  free (map);
}

void
map_gb_set_cpu (MapGB *map, CpuGB *cpu) {
  map->cpu = cpu;
}

void
map_gb_set_rom (MapGB *map, RomGB *rom) {
  map->rom = rom;
}

BYTE
map_gb_get_memory (MapGB *map, ADDR16 addr) {
  BYTE result;
  printf ("%s : $%04x\n", FUNC, addr);

  if (map->rom == NULL || map->cpu == NULL)
    printf ("%s : rom or map are not set!\n", FUNC);

  if (addr < 0x4000)
    result = rom_gb_get_rom_memory (map->rom, 0, addr);
  else if (addr < 0x8000)
    result = rom_gb_get_rom_memory (map->rom, 1, addr);
  else if (addr < 0xA000) {
    printf ("%s : access to video ram at [%04x] not implemented!\n", FUNC, addr);
    result = 0x00;
  }
  else if (addr < 0xC000) {
    printf ("%s : access to switchable ram at [%04x] not implemented!\n", FUNC, addr);
    result = 0x00;
  }
  else if (addr < 0xE000)
    result = cpu_gb_get_ram_memory (map->cpu, addr - 0xC000);
  else if (addr < 0xFE00)
    result = cpu_gb_get_ram_memory (map->cpu, addr - 0xE000);
  else if (addr < 0xFEA0) {
    printf ("%s : not implemented!\n", FUNC);
    result = 0x00;
  }
  else if (addr < 0xFF00) {
    printf ("%s : not implemented!\n", FUNC);
    result = 0x00;
  }
  else if (addr < 0xFF4C) {
    printf ("%s : not implemented!\n", FUNC);
    result = 0x00;
  }
  else if (addr < 0xFF80) {
    printf ("%s : not implemented!\n", FUNC);
    result = 0x00;
  }
  else if (addr < 0xFFFF) {
    /*printf ("%s : access to switchable ram at [%04x] not implemented!\n", FUNC, addr);
    result = 0x00;*/
      result = map->tmp[addr - 0xFF80];
  }
  else {
    printf ("%s : access to Interrupt Enable Register not implemented!\n", FUNC);
    result = 0x00;
  }

  return result;
}

void
map_gb_set_memory (MapGB *map, ADDR16 addr, BYTE value) {
  printf ("%s : $%04x %02x\n", FUNC, addr, value);
  if (addr >= 0xFF80 && addr < 0xFFFF)
    map->tmp[addr - 0xFF80] = value;
}
