#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

typedef void (*CPUExecOp) (CPU *cpu);

struct _CPU {
  REG8 reg_a;
  REG8 reg_x;
  REG8 reg_y;
  REG8 reg_status;
  REG8 reg_sp;
  REG16 reg_ip;
  BYTE ram[0x0800]; // 2KB
  Rom *rom;
  Mapper *mapper;
};

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

Rom *
cpu_get_rom (CPU *cpu) {
  return cpu->rom;
}

void
cpu_set_mapper (CPU *cpu, Mapper *mapper) {
  cpu->mapper = mapper;
}

Mapper *
cpu_get_mapper (CPU *cpu) {
  return cpu->mapper;
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
cpu_push (CPU *cpu, BYTE value) {

}

BYTE
cpu_pop (CPU *cpu) {
  return 0;
}

BOOL
cpu_get_carry_flag (CPU *cpu) {
  return cpu->reg_status & 0x01;
}

void
cpu_set_carry_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x01 : cpu->reg_status & ~0x01;
}

BOOL
cpu_get_zero_flag (CPU *cpu) {
  return cpu->reg_status & 0x02;
}

void
cpu_set_zero_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x02 : cpu->reg_status & ~0x02;
}

BOOL
cpu_get_interrupt_disable_flag (CPU *cpu) {
  return cpu->reg_status & 0x04;
}

void
cpu_set_interrupt_disable_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x04 : cpu->reg_status & ~0x04;
}

BOOL
cpu_get_decimal_flag (CPU *cpu) {
  return cpu->reg_status & 0x08;
}

void
cpu_set_decimal_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x08 : cpu->reg_status & ~0x08;
}

BOOL
cpu_get_break_flag (CPU *cpu) {
  return cpu->reg_status & 0x10;
}

void
cpu_set_break_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x10 : cpu->reg_status & ~0x10;
}

BOOL
cpu_get_overflow_flag (CPU *cpu) {
  return cpu->reg_status & 0x40;
}

void
cpu_set_overflow_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x40 : cpu->reg_status & ~0x40;
}

BOOL
cpu_get_negative_flag (CPU *cpu) {
  return cpu->reg_status & 0x80;
}

void
cpu_set_negative_flag (CPU *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x80 : cpu->reg_status & ~0x80;
}

void
cpu_exec_null (CPU *cpu) {
  printf ("cpu_exec_null\n");
}

void
cpu_exec_brk (CPU *cpu) {
  printf ("cpu_exec_brk\n");
  cpu->reg_ip++;
  cpu_push (cpu, (cpu->reg_ip >> 8) & 0xff);
  cpu_push (cpu, cpu->reg_ip & 0xff);
  cpu_set_break_flag (cpu, TRUE);
  cpu_push (cpu, cpu->reg_status);
  cpu_set_interrupt_disable_flag (cpu, TRUE);
  //cpu->reg_ip = cpu_get_memory (cpu, 0xFFFE) | (cpu_get_memory (cpu, 0xFFFF) << 8);
}

const struct {
  char *name;
  unsigned int cycle;
  CPUExecOp func;
} OpCodes[] = {
  { "BRK", 7, cpu_exec_brk }, // 0x00
  { "ORA", 1, cpu_exec_null }, // 0x01
  { "?", 1, cpu_exec_null }, // 0x02
  { "?", 1, cpu_exec_null }, // 0x03
  { "?", 1, cpu_exec_null }, // 0x04
  { "ORA", 1, cpu_exec_null }, // 0x05
  { "ASL", 1, cpu_exec_null }, // 0x06
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

void
cpu_step (CPU *cpu) {
  INT8 opcode;

  opcode = rom_get_prg_memory (cpu->rom, cpu->reg_ip);

  printf ("\tcpu_step : [opcode] %02x\n", opcode & 0xff);
  if ((opcode & 0xff) < 0x20) {
    printf ("\t\tcpu_step : %s\n", OpCodes[(int) opcode].name);
  }
  if ((opcode & 0xff) == 0x00)
    OpCodes[(int) opcode].func (cpu);
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
