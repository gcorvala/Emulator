#include "cpu_6502.h"
#include <stdio.h>

enum {
  OP_6502_LDX_I   = 0xa2,
  OP_6502_LDA_I   = 0xa9,
  OP_6502_SEI     = 0x78,
  OP_6502_STA_A   = 0x8d,
  OP_6502_CLD     = 0xd8,
  OP_6502_DEFAULT = 0x00
} OpCodes;

void
cpu_6502_load_rom (CPU6502 *cpu, RomNES *rom) {
  cpu->bank1 = rom->prg;
  cpu->bank2 = rom->chr;

  cpu->SP = cpu->bank1;
}

void
cpu_6502_step (CPU6502 *cpu) {
  printf ("\tcpu_6502_step : [opcode] %x\n", *(cpu->SP));
  switch (*(cpu->SP) & 0xff) {
    case OP_6502_LDX_I:
      cpu->X = *(cpu->SP++) & 0xff;
    case OP_6502_LDA_I:
      cpu->A = *(cpu->SP++) & 0xff;
    case OP_6502_SEI:
      cpu->I = 1;
      break;
    case OP_6502_STA_A:
      cpu->SP += 2;
      break;
    case OP_6502_CLD:
      cpu->D = 0;
      break;
    default:
      printf ("\t\t not yet implemented!\n");
      break;
  }
  cpu->SP++;
}
