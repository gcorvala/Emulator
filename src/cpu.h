#ifndef _CPU_H_
#define _CPU_H_

#include "mapper.h"
#include "rom.h"
#include "types.h"

typedef struct {
  BYTE A;
  BYTE X;
  BYTE Y;
  int C : 1;
  int Z : 1;
  int I : 1;
  int D : 1;
  int B : 1;
  int V : 1;
  int S : 1;
  ADDR IP;
  ADDR SP;
  BYTE ram[0x0800]; // 2KB
  // stack stored at $0100-$01FF
  Rom *rom;
  Mapper *mapper;
} CPU;

CPU  * cpu_new        (void);
void   cpu_free       (CPU *cpu);
void   cpu_load_rom   (CPU *cpu, Rom *rom);
void   cpu_set_mapper (CPU *cpu, Mapper *mapper);
void   cpu_step       (CPU *cpu);
BYTE   cpu_get_memory (CPU *cpu, ADDR addr);
void   cpu_set_memory (CPU *cpu, ADDR addr, BYTE value);

#endif
