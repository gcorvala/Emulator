#include "cpu.h"
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

CPU *
cpu_new (void) {
  CPU *cpu;

  cpu = calloc (sizeof (CPU), 1);

  return cpu;
}

void
cpu_load_rom (CPU *cpu, RomNES *rom) {
  cpu->SP = 0;
  cpu->rom = rom;
}

void
cpu_step (CPU *cpu) {
  printf ("\tcpu_step : [opcode] %02x\n", * (cpu->rom->prg + cpu->SP) & 0xff);
  switch (cpu->rom->prg[cpu->SP] & 0xff) {
    case OP_2A03_LDX_I:
      cpu->X = cpu->rom->prg[cpu->SP++];
      break;
    case OP_2A03_LDA_I:
      cpu->A = cpu->rom->prg[cpu->SP++];
      break;
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

BYTE
cpu_get_memory (CPU *cpu, ADDR addr) {
  BYTE value = 0;

  if (addr < 0x0800) {
    value = *(cpu->ram + addr);
  }
  else {
    printf ("\tcpu_get_memory at %04x not yet implemented !\n", addr);
  }

  return value;
}

void
cpu_set_memory (CPU *cpu, ADDR addr, BYTE value) {
  if (addr < 0x800) {
    cpu->ram[addr] = value;
  }
  else {
    printf ("\tcpu_set_memory at %04x not yet implemented !\n", addr);
  }
}

