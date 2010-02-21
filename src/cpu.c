#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

struct _CPU {
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
};

enum {
  OP_2A03_LDX_I   = 0xa2,
  OP_2A03_LDA_I   = 0xa9,
  OP_2A03_SEI     = 0x78,
  OP_2A03_STA_A   = 0x8d,
  OP_2A03_CLD     = 0xd8,
  OP_2A03_DEFAULT = 0x00
} OpCodes;

BYTE
cpu_get_memory (CPU *cpu, ADDR addr) {
  BYTE value = 0;

  if (addr < 0x2000) {
    ADDR real_addr;
    
    real_addr = addr % 0x0800;

    value = *(cpu->ram + real_addr);
  }
  else if (addr == 0x2000) {
    printf ("\tcpu_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr == 0x2001) {
    printf ("\tcpu_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr >= 0x8000) {
    value = mapper_get_memory (cpu->mapper, addr);
  }
  else {
    printf ("\tcpu_get_memory at %04x not yet implemented !\n", addr);
  }

  return value;
}

void
cpu_set_memory (CPU *cpu, ADDR addr, BYTE value) {
  if (addr < 0x2000) {
    ADDR real_addr;

    real_addr = addr % 0x0800;

    cpu->ram[real_addr] = value;
  }
  else {
    printf ("\tcpu_set_memory at %04x not yet implemented !\n", addr);
  }
}

CPU *
cpu_new (void) {
  CPU *cpu;

  cpu = calloc (sizeof (CPU), 1);

  cpu->SP = 0x01FF;

  return cpu;
}

void
cpu_free (CPU *cpu) {
  free (cpu);
}

void
cpu_set_rom (CPU *cpu, Rom *rom) {
  cpu->SP = 0;
  cpu->rom = rom;
}

void
cpu_set_mapper (CPU *cpu, Mapper *mapper) {
  cpu->mapper = mapper;
}

void
cpu_step (CPU *cpu) {
  printf ("\tcpu_step : [opcode] %02x\n", rom_get_prg_memory (cpu->rom, cpu->SP) & 0xff);
  switch (rom_get_prg_memory (cpu->rom, cpu->SP) & 0xff) {
    case OP_2A03_LDX_I:
      cpu->X = rom_get_prg_memory (cpu->rom, cpu->SP++);
      break;
    case OP_2A03_LDA_I:
      cpu->A = rom_get_prg_memory (cpu->rom, cpu->SP++);
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
