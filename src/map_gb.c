#include "map_gb.h"

#include "cpu_gb.h"
#include "rom_gb.h"
#include <stdlib.h>
#include <stdio.h>

struct _MapGB {
  CpuGB *cpu;
  RomGB *rom;
  BYTE video_ram[0x2000];
  BOOL bootrom;
  BYTE io_reg[0x007F];
  BYTE interrupts;
};

MapGB *
map_gb_new (void) {
  MapGB *map;

  map = calloc (sizeof (MapGB), 1);

  map->bootrom = TRUE;

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
  /*printf ("%s : $%04x\n", FUNC, addr);*/

  if (addr < 0x4000) {
    if (addr < 0x100 && map->bootrom == TRUE)
      result = cpu_gb_get_rom_memory (map->cpu, addr);
    else {
      result = rom_gb_get_rom_memory (map->rom, 0, addr);
    }
  }
  else if (addr >= 0x4000 && addr < 0x8000)
    result = rom_gb_get_rom_memory (map->rom, 1, addr % 0x4000);
  else if (addr >= 0x8000 && addr < 0xA000)
    result = map->video_ram[addr - 0x8000];
  else if (addr >= 0xC000 && addr < 0xE000)
    result = cpu_gb_get_ram_memory (map->cpu, addr - 0xC000);
  else if (addr >= 0xFF00 && addr < 0xFF7F) {
    if (addr == 0xFF44 && map->bootrom == TRUE)
      result = 0x90;
    else if (addr == 0xFF44)
      result = 0x91;
    else
      result = map->io_reg[addr & 0x00FF];
  }
  else if (addr >= 0xFF80 && addr < 0xFFFF)
    result = cpu_gb_get_hram_memory (map->cpu, addr - 0xFF80);
  else if (addr == 0xFFFF)
    result = map->interrupts;
  else {
    printf ("!!! READ MEMORY NOT YET IMPLEMENTED!!! ");
    printf ("%s : $%04x\n", FUNC, addr);
    result = 0x00;
  }

  return result;
}

void
map_gb_set_memory (MapGB *map, ADDR16 addr, BYTE value) {
  printf ("%s : $%04x %02x\n", FUNC, addr, value);
  if (addr >= 0x8000 && addr <= 0xA000)
    map->video_ram[addr - 0x8000] = value;
  else if (addr >= 0xC000 && addr < 0xE000)
    cpu_gb_set_ram_memory (map->cpu, addr - 0xC000, value);
  else if (addr >= 0xFF00 && addr < 0xFF7F) {
    if (addr == 0xFF50 && value == 0x01)
      map->bootrom = FALSE;
    map->io_reg[addr & 0x00FF] = value;
  }
  else if (addr >= 0xFF80 && addr < 0xFFFF)
    cpu_gb_set_hram_memory (map->cpu, addr - 0xFF80, value);
  else if (addr == 0xFFFF)
    map->interrupts = value;
  else {
    printf ("!!! WRITE MEMORY NOT YET IMPLEMENTED!!! ");
    printf ("%s : $%04x %02x\n", FUNC, addr, value);
  }
}
