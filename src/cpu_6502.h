#ifndef _CPU_6502_H_
#define _CPU_6502_H_

#include "rom_nes.h"
#include "types.h"

typedef struct {
  BYTE ram[2048];
  BYTE ppu_reg[8];
  BYTE apu_reg[20];
  BYTE exp_rom[8159];
  BYTE sram[8192];
  BYTE bank1[16384];
  BYTE bank2[16384];
} Memory;

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
  Memory *mem;
} CPU6502;

CPU6502 * cpu_6502_new      (void);
void      cpu_6502_load_rom (CPU6502 *cpu, RomNES *rom);
void      cpu_6502_step     (CPU6502 *cpu);

#endif
