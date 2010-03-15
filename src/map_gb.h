#ifndef _MAP_GB_H_
#define _MAP_GB_H_

#include "types.h"

MapGB * map_gb_new        (void);
void    map_gb_free       (MapGB *map);
void    map_gb_set_cpu    (MapGB *map, CpuGB *cpu);
void    map_gb_set_rom    (MapGB *map, RomGB *rom);
BYTE    map_gb_get_memory (MapGB *map, ADDR16 addr);
void    map_gb_set_memory (MapGB *map, ADDR16 addr, BYTE value);

#endif