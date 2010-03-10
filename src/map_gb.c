#include "map_gb.h"

#include "cpu_gb.h"
#include "rom_gb.h"
#include <stdlib.h>
#include <stdio.h>

struct _MapGB {
  CpuGB *cpu;
  RomGB *rom;
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

  if (map->rom == NULL || map->cpu == NULL)
    printf ("%s : rom or map are not set!\n", FUNC);

  if (addr < 0x4000)
    result = rom_gb_get_memory (map->rom, 0, addr);
  else if (addr < 0x8000)
    result = rom_gb_get_memory (map->rom, 1, addr);
  else
    result = 0x00;

  return result;
}

void
map_gb_set_memory (MapGB *map, ADDR16 addr, BYTE value) {
  
}