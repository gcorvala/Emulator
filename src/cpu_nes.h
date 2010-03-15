#ifndef _CPU_NES_H_
#define _CPU_NES_H_

#include "types.h"

CpuNES *  cpu_nes_new                        (void);
void      cpu_nes_free                       (CpuNES *cpu);
void      cpu_nes_set_rom                    (CpuNES *cpu, RomNES *rom);
RomNES *  cpu_nes_get_rom                    (CpuNES *cpu);
void      cpu_nes_set_mapper                 (CpuNES *cpu, MapNES *mapper);
MapNES *  cpu_nes_get_mapper                 (CpuNES *cpu);
void      cpu_nes_set_memory                 (CpuNES *cpu, ADDR16 addr, BYTE value);
BYTE      cpu_nes_get_memory                 (CpuNES *cpu, ADDR16 addr);
void      cpu_nes_push                       (CpuNES *cpu, BYTE value);
BYTE      cpu_nes_pop                        (CpuNES *cpu);
BOOL      cpu_nes_get_carry_flag             (CpuNES *cpu);
void      cpu_nes_set_carry_flag             (CpuNES *cpu, BOOL flag);
BOOL      cpu_nes_get_zero_flag              (CpuNES *cpu);
void      cpu_nes_set_zero_flag              (CpuNES *cpu, BOOL flag);
BOOL      cpu_nes_get_interrupt_disable_flag (CpuNES *cpu);
void      cpu_nes_set_interrupt_disable_flag (CpuNES *cpu, BOOL flag);
BOOL      cpu_nes_get_decimal_flag           (CpuNES *cpu);
void      cpu_nes_set_decimal_flag           (CpuNES *cpu, BOOL flag);
BOOL      cpu_nes_get_break_flag             (CpuNES *cpu);
void      cpu_nes_set_break_flag             (CpuNES *cpu, BOOL flag);
BOOL      cpu_nes_get_overflow_flag          (CpuNES *cpu);
void      cpu_nes_set_overflow_flag          (CpuNES *cpu, BOOL flag);
BOOL      cpu_nes_get_negative_flag          (CpuNES *cpu);
void      cpu_nes_set_negative_flag          (CpuNES *cpu);
void      cpu_nes_step                       (CpuNES *cpu);

#endif
