#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

struct _CPU {
  REG8 reg_a;
  REG8 reg_x;
  REG8 reg_y;
  REG8 reg_sp;
  REG16 reg_ip;
  int carry_flag : 1;
  int zero_flag : 1;
  int interrupt_disable_flag : 1;
  int decimal_mode : 1;
  int break_command : 1;
  int overflow_flag : 1;
  int negative_flag : 1;
  BYTE ram[0x0800]; // 2KB
  Rom *rom;
  Mapper *mapper;
};

const struct {
  char *name;
  unsigned int cycle;
} OpCodes[] = {
  { "BRK", 7 }, // 0x00
  { "ORA", 1 }, // 0x01
  { "?", 1 }, // 0x02
  { "?", 1 }, // 0x03
  { "?", 1 }, // 0x04
  { "ORA", 1 }, // 0x05
  { "ASL", 1 }, // 0x06
  { "?", 1 }, // 0x07
  { "PHP", 1 }, // 0x08
  { "ORA", 1 }, // 0x09
  { "ASL", 1 }, // 0x0A
  { "?", 1 }, // 0x0B
  { "?", 1 }, // 0x0C
  { "ORA", 1 }, // 0x0D
  { "ASL", 1 }, // 0x0E
  { "?", 1 }, // 0x0F
  { "BPL", 7 }, // 0x10
  { "ORA", 1 }, // 0x11
  { "?", 1 }, // 0x12
  { "?", 1 }, // 0x13
  { "?", 1 }, // 0x14
  { "ORA", 1 }, // 0x15
  { "ASL", 1 }, // 0x16
  { "?", 1 }, // 0x17
  { "CLC", 1 }, // 0x18
  { "ORA", 1 }, // 0x19
  { "?", 1 }, // 0x1A
  { "?", 1 }, // 0x1B
  { "?", 1 }, // 0x1C
  { "ORA", 1 }, // 0x1D
  { "ASL", 1 }, // 0x1E
  { "?", 1 }, // 0x1F
};


/*enum {
  OP_2A03_LDX_I   = 0xa2,
  OP_2A03_LDA_I   = 0xa9,
  OP_2A03_SEI     = 0x78,
  OP_2A03_STA_A   = 0x8d,
  OP_2A03_CLD     = 0xd8,
  OP_2A03_DEFAULT = 0x00
} OpCodes;*/

BYTE
cpu_get_memory (CPU *cpu, ADDR16 addr) {
  BYTE value = 0;

  if (addr < 0x2000) {
    ADDR16 real_addr;
    
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
cpu_set_memory (CPU *cpu, ADDR16 addr, BYTE value) {
  if (addr < 0x2000) {
    ADDR16 real_addr;

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

  cpu->reg_sp = 0x00FF;

  return cpu;
}

void
cpu_free (CPU *cpu) {
  free (cpu);
}

void
cpu_set_rom (CPU *cpu, Rom *rom) {
  cpu->reg_ip = 0;
  cpu->rom = rom;
}

void
cpu_set_mapper (CPU *cpu, Mapper *mapper) {
  cpu->mapper = mapper;
}

void
cpu_step (CPU *cpu) {
  INT8 opcode;

  opcode = rom_get_prg_memory (cpu->rom, cpu->reg_ip);

  printf ("\tcpu_step : [opcode] %02x\n", opcode & 0xff);
  if ((opcode & 0xff) < 0x20) {
    printf ("\t\tcpu_step : %s\n", OpCodes[(int) opcode].name);
  }
  /*switch (rom_get_prg_memory (cpu->rom, cpu->reg_sp) & 0xff) {
    case OP_2A03_LDX_I:
      cpu->reg_x = rom_get_prg_memory (cpu->rom, cpu->reg_sp++);
      break;
    case OP_2A03_LDA_I:
      cpu->reg_a = rom_get_prg_memory (cpu->rom, cpu->reg_sp++);
      break;
    case OP_2A03_SEI:
      cpu->interrupt_disable_flag = 1;
      break;
    case OP_2A03_STA_A:
      cpu->reg_sp += 2;
      break;
    case OP_2A03_CLD:
      cpu->decimal_mode = 0;
      break;
    default:
      printf ("\t\t not yet implemented!\n");
      break;
  }*/
  cpu->reg_ip++;
}
