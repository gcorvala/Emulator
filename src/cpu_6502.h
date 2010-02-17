#ifndef _CPU_6502_H_
#define _CPU_6502_H_

#include "rom_nes.h"
#include "types.h"

typedef struct {
  BYTE A;     // Accumulator
  BYTE X;     //
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
  BYTE *bank1;
  BYTE *bank2;
} CPU6502;

void    cpu_6502_load_rom   (CPU6502 *cpu, RomNES *rom);
void    cpu_6502_step       (CPU6502 *cpu);

#endif