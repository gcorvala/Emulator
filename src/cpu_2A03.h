#ifndef _CPU_2A03_H_
#define _CPU_2A03_H_

#include "rom_nes.h"
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
  BYTE *IP;
  BYTE *SP;
  BYTE ram[0x800]; // 2KB
} CPU2A03;

CPU2A03 * cpu_2A03_new      (void);
void      cpu_2A03_load_rom (CPU2A03 *cpu, RomNES *rom);
void      cpu_2A03_step     (CPU2A03 *cpu);

#endif
