#ifndef _CPU6502_H_
#define _CPU6502_H_

#include "mapper.h"
#include "rom_nes.h"
#include "types.h"

typedef struct _CPU6502 CPU6502;

CPU6502 * cpu_6502_new                        (void);
void      cpu_6502_free                       (CPU6502 *cpu);
void      cpu_6502_set_rom                    (CPU6502 *cpu, RomNES *rom);
RomNES *     cpu_6502_get_rom                    (CPU6502 *cpu);
void      cpu_6502_set_mapper                 (CPU6502 *cpu, Mapper *mapper);
Mapper *  cpu_6502_get_mapper                 (CPU6502 *cpu);
void      cpu_6502_set_memory                 (CPU6502 *cpu, ADDR16 addr, BYTE value);
BYTE      cpu_6502_get_memory                 (CPU6502 *cpu, ADDR16 addr);
void      cpu_6502_push                       (CPU6502 *cpu, BYTE value);
BYTE      cpu_6502_pop                        (CPU6502 *cpu);
BOOL      cpu_6502_get_carry_flag             (CPU6502 *cpu);
void      cpu_6502_set_carry_flag             (CPU6502 *cpu, BOOL flag);
BOOL      cpu_6502_get_zero_flag              (CPU6502 *cpu);
void      cpu_6502_set_zero_flag              (CPU6502 *cpu, BOOL flag);
BOOL      cpu_6502_get_interrupt_disable_flag (CPU6502 *cpu);
void      cpu_6502_set_interrupt_disable_flag (CPU6502 *cpu, BOOL flag);
BOOL      cpu_6502_get_decimal_flag           (CPU6502 *cpu);
void      cpu_6502_set_decimal_flag           (CPU6502 *cpu, BOOL flag);
BOOL      cpu_6502_get_break_flag             (CPU6502 *cpu);
void      cpu_6502_set_break_flag             (CPU6502 *cpu, BOOL flag);
BOOL      cpu_6502_get_overflow_flag          (CPU6502 *cpu);
void      cpu_6502_set_overflow_flag          (CPU6502 *cpu, BOOL flag);
BOOL      cpu_6502_get_negative_flag          (CPU6502 *cpu);
void      cpu_6502_set_negative_flag          (CPU6502 *cpu, BOOL flag);
void      cpu_6502_step                       (CPU6502 *cpu);

#endif
