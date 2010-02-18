#include "cpu_2A03.h"
#include <stdlib.h>
#include <stdio.h>

enum {
  OP_2A03_LDX_I   = 0xa2,
  OP_2A03_LDA_I   = 0xa9,
  OP_2A03_SEI     = 0x78,
  OP_2A03_STA_A   = 0x8d,
  OP_2A03_CLD     = 0xd8,
  OP_2A03_DEFAULT = 0x00
} OpCodes;

CPU2A03 *
cpu_2A03_new (void) {
  CPU2A03 *cpu;

  cpu = calloc (sizeof (CPU2A03), 1);

  return cpu;
}

void
cpu_2A03_load_rom (CPU2A03 *cpu, RomNES *rom) {
  cpu->SP = rom->prg;
}

void
cpu_2A03_step (CPU2A03 *cpu) {
  printf ("\tcpu_2A03_step : [opcode] %02x\n", *(cpu->SP) & 0xff);
  switch (*(cpu->SP) & 0xff) {
    case OP_2A03_LDX_I:
      cpu->X = *(cpu->SP++) & 0xff;
    case OP_2A03_LDA_I:
      cpu->A = *(cpu->SP++) & 0xff;
    case OP_2A03_SEI:
      cpu->I = 1;
      break;
    case OP_2A03_STA_A:
      cpu->SP += 2;
      break;
    case OP_2A03_CLD:
      cpu->D = 0;
      break;
    default:
      printf ("\t\t not yet implemented!\n");
      break;
  }
  cpu->SP++;
}
