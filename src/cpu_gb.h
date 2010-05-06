#ifndef _CPU_GB_H_
#define _CPU_GB_H_

#include "types.h"

CpuGB * cpu_gb_new                 (void);
BYTE    cpu_gb_get_ram_memory      (CpuGB *cpu, ADDR16 addr);
void    cpu_gb_set_ram_memory      (CpuGB *cpu, ADDR16 addr, BYTE value);
BYTE    cpu_gb_get_hram_memory     (CpuGB *cpu, ADDR16 addr);
void    cpu_gb_set_hram_memory     (CpuGB *cpu, ADDR16 addr, BYTE value);
BYTE    cpu_gb_get_rom_memory      (CpuGB *cpu, ADDR16 addr);
void    cpu_gb_set_mapper          (CpuGB *cpu, MapGB *map);
void    cpu_gb_interrupt           (CpuGB *cpu);
void    cpu_gb_update_clock        (CpuGB *cpu, UINT32 cycles);
#endif
