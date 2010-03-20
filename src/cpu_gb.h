#ifndef _CPU_GB_H_
#define _CPU_GB_H_

#include "types.h"

CpuGB * cpu_gb_new                 (void);
void    cpu_gb_free                (CpuGB *cpu);
BYTE    cpu_gb_get_ram_memory      (CpuGB *cpu, ADDR16 addr);
void    cpu_gb_set_ram_memory      (CpuGB *cpu, ADDR16 addr, BYTE value);
BYTE    cpu_gb_get_rom_memory      (CpuGB *cpu, ADDR16 addr);
void    cpu_gb_set_zero_flag       (CpuGB *cpu);
void    cpu_gb_set_substract_flag  (CpuGB *cpu);
void    cpu_gb_set_half_carry_flag (CpuGB *cpu);
void    cpu_gb_set_carry_flag      (CpuGB *cpu);

void    cpu_gb_set_mapper          (CpuGB *cpu, MapGB *map);

void    cpu_gb_step                (CpuGB *cpu);

#endif
