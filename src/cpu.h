#ifndef _CPU_H_
#define _CPU_H_

#include "mapper.h"
#include "rom.h"
#include "types.h"

typedef struct _CPU CPU;

CPU *    cpu_new                        (void);
void     cpu_free                       (CPU *cpu);
void     cpu_set_rom                    (CPU *cpu, Rom *rom);
Rom *    cpu_get_rom                    (CPU *cpu);
void     cpu_set_mapper                 (CPU *cpu, Mapper *mapper);
Mapper * cpu_get_mapper                 (CPU *cpu);
void     cpu_set_memory                 (CPU *cpu, ADDR16 addr, BYTE value);
BYTE     cpu_get_memory                 (CPU *cpu, ADDR16 addr);
void     cpu_push                       (CPU *cpu, BYTE value);
BYTE     cpu_pop                        (CPU *cpu);
BOOL     cpu_get_carry_flag             (CPU *cpu);
void     cpu_set_carry_flag             (CPU *cpu, BOOL flag);
BOOL     cpu_get_zero_flag              (CPU *cpu);
void     cpu_set_zero_flag              (CPU *cpu, BOOL flag);
BOOL     cpu_get_interrupt_disable_flag (CPU *cpu);
void     cpu_set_interrupt_disable_flag (CPU *cpu, BOOL flag);
BOOL     cpu_get_decimal_flag           (CPU *cpu);
void     cpu_set_decimal_flag           (CPU *cpu, BOOL flag);
BOOL     cpu_get_break_flag             (CPU *cpu);
void     cpu_set_break_flag             (CPU *cpu, BOOL flag);
BOOL     cpu_get_overflow_flag          (CPU *cpu);
void     cpu_set_overflow_flag          (CPU *cpu, BOOL flag);
BOOL     cpu_get_negative_flag          (CPU *cpu);
void     cpu_set_negative_flag          (CPU *cpu, BOOL flag);
void     cpu_step                       (CPU *cpu);

#endif
