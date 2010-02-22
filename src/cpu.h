#ifndef _CPU_H_
#define _CPU_H_

#include "mapper.h"
#include "rom.h"
#include "types.h"

typedef struct _CPU CPU;

CPU *    cpu_new        (void);
void     cpu_free       (CPU *cpu);
void     cpu_set_rom    (CPU *cpu, Rom *rom);
Rom *    cpu_get_rom    (CPU *cpu);
void     cpu_set_mapper (CPU *cpu, Mapper *mapper);
Mapper * cpu_get_mapper (CPU *cpu);
void     cpu_set_memory (CPU *cpu, ADDR16 addr, BYTE value);
BYTE     cpu_get_memory (CPU *cpu, ADDR16 addr);

void     cpu_step       (CPU *cpu);

#endif
