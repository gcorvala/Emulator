#include "cpu_nes.h"

#include "map_nes.h"
#include "rom_nes.h"

#include <stdlib.h>
#include <stdio.h>

typedef void (*CpuNESExecOp) (CpuNES *cpu);

struct _CpuNES {
  REG8 reg_a;
  REG8 reg_x;
  REG8 reg_y;
  REG8 reg_status;
  REG8 reg_sp; /* $0100-$01FF */
  REG16 reg_ip;
  BYTE ram[0x0800]; /* 2KB */
  RomNES *rom;
  MapNES *mapper;
};

typedef enum {
  ADDR_MODE_IMMEDIATE,
  ADDR_MODE_ABSOLUTE,
  ADDR_MODE_ZERO_PAGE_ABSOLUTE,
  ADDR_MODE_IMPLIED,
  ADDR_MODE_ACCUMULATOR,
  ADDR_MODE_INDEXED,
  ADDR_MODE_ZERO_PAGE_INDEXED,
  ADDR_MODE_INDIRECT,
  ADDR_MODE_PRE_INDEXED_INDIRECT,
  ADDR_MODE_POST_INDEXED_INDIRECT,
  ADDR_MODE_RELATIVE
} AddressingMode;

typedef enum {
  CpuNES_OPCODE_00 = 0,
  CpuNES_OPCODE_01 = 1,
  CpuNES_OPCODE_10 = 2,
  CpuNES_OPCODE_11 = 3
} OpCode;

CpuNES *
cpu_nes_new (void) {
  CpuNES *cpu;

  cpu = calloc (sizeof (CpuNES), 1);

  cpu->reg_sp = (REG8) 0xFF;

  return cpu;
}

void
cpu_nes_free (CpuNES *cpu) {
  free (cpu);
}

void
cpu_nes_set_rom (CpuNES *cpu, RomNES *rom) {
  cpu->reg_ip.r_16 = 0;
  cpu->rom = rom;
}

RomNES *
cpu_nes_get_rom (CpuNES *cpu) {
  return cpu->rom;
}

void
cpu_nes_set_mapper (CpuNES *cpu, MapNES *mapper) {
  cpu->mapper = mapper;
}

MapNES *
cpu_nes_get_mapper (CpuNES *cpu) {
  return cpu->mapper;
}

void
cpu_nes_set_memory (CpuNES *cpu, ADDR16 addr, BYTE value) {
  if (addr < 0x2000) {
    ADDR16 real_addr;

    real_addr = addr % 0x0800;

    cpu->ram[real_addr] = value;
  }
  else {
    printf ("\tcpu_nes_set_memory at %04x not yet implemented !\n", addr);
  }
}

BYTE
cpu_nes_get_memory (CpuNES *cpu, ADDR16 addr) {
  BYTE value = 0;

  if (addr < 0x2000) {
    ADDR16 real_addr;
    
    real_addr = addr % 0x0800;

    value = *(cpu->ram + real_addr);
  }
  else if (addr == 0x2000) {
    printf ("\tcpu_nes_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr == 0x2001) {
    printf ("\tcpu_nes_get_memory at %04x is not allowed !\n", addr);
  }
  else if (addr >= 0x8000) {
    value = map_nes_get_memory (cpu->mapper, addr);
  }
  else {
    printf ("\tcpu_nes_get_memory at %04x not yet implemented !\n", addr);
  }

  return value;
}

void
cpu_nes_push (CpuNES *cpu, BYTE value) {
  cpu_nes_set_memory (cpu, 0x0100 | (cpu->reg_sp & 0xff), value);
  cpu->reg_sp--;
}

BYTE
cpu_nes_pop (CpuNES *cpu) {
  return cpu_nes_get_memory (cpu, 0x0100 | (cpu->reg_sp & 0xff));
  cpu->reg_sp++;
}

BOOL
cpu_nes_get_carry_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x01;
}

void
cpu_nes_set_carry_flag (CpuNES *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x01 : cpu->reg_status & ~0x01;
}

BOOL
cpu_nes_get_zero_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x02;
}

void
cpu_nes_set_zero_flag (CpuNES *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x02 : cpu->reg_status & ~0x02;
}

BOOL
cpu_nes_get_interrupt_disable_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x04;
}

void
cpu_nes_set_interrupt_disable_flag (CpuNES *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x04 : cpu->reg_status & ~0x04;
}

BOOL
cpu_nes_get_decimal_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x08;
}

void
cpu_nes_set_decimal_flag (CpuNES *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x08 : cpu->reg_status & ~0x08;
}

BOOL
cpu_nes_get_break_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x10;
}

void
cpu_nes_set_break_flag (CpuNES *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x10 : cpu->reg_status & ~0x10;
}

BOOL
cpu_nes_get_overflow_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x40;
}

void
cpu_nes_set_overflow_flag (CpuNES *cpu, BOOL flag) {
  cpu->reg_status = flag ? cpu->reg_status | 0x40 : cpu->reg_status & ~0x40;
}

BOOL
cpu_nes_get_negative_flag (CpuNES *cpu) {
  return cpu->reg_status & 0x80;
}

void
cpu_nes_set_negative_flag (CpuNES *cpu) {
  cpu->reg_status = (cpu->reg_a & 0x80) ? cpu->reg_status | 0x80 : cpu->reg_status & ~0x80;
}

void
cpu_nes_exec_null (CpuNES *cpu) {
  printf ("cpu_nes_exec_null\n");
}

void
cpu_nes_exec_brk (CpuNES *cpu) {
  printf ("cpu_nes_exec_brk\n");
  cpu->reg_ip.r_16++;
  cpu_nes_push (cpu, cpu->reg_ip.r_8.h);
  cpu_nes_push (cpu, cpu->reg_ip.r_8.l);
  cpu_nes_set_break_flag (cpu, TRUE);
  cpu_nes_push (cpu, cpu->reg_status);
  cpu_nes_set_interrupt_disable_flag (cpu, TRUE);
  /*printf ("a%04xa\n", (cpu_nes_get_memory (cpu, 0xFFFE) | (cpu_nes_get_memory (cpu, 0xFFFF) << 8)) & 0xFFFF);
  cpu->reg_ip = cpu_nes_get_memory (cpu, 0xFFFE) | (cpu_nes_get_memory (cpu, 0xFFFF) << 8); */
}

void
cpu_nes_exec_cld (CpuNES *cpu) {
  printf ("cpu_nes_exec_cld\n");
  cpu_nes_set_decimal_flag (cpu, FALSE);
}

void
cpu_nes_exec_lda (CpuNES *cpu) {
  printf ("cpu_nes_exec_lda %02x\n", rom_nes_get_prg_memory (cpu->rom, cpu->reg_ip.r_16 + 1));
  cpu->reg_a = rom_nes_get_prg_memory (cpu->rom, cpu->reg_ip.r_16 + 1);
  cpu_nes_set_zero_flag (cpu, cpu->reg_a == 0 ? TRUE : FALSE);
  cpu_nes_set_negative_flag (cpu);
}

void
cpu_nes_exec_ldx (CpuNES *cpu) {
  printf ("cpu_nes_exec_ldx\n");

  cpu->reg_x = rom_nes_get_prg_memory (cpu->rom, cpu->reg_ip.r_16 + 1);
  cpu_nes_set_zero_flag (cpu, cpu->reg_x == 0 ? TRUE : FALSE);
  cpu_nes_set_negative_flag (cpu);
}

void
cpu_nes_exec_sei (CpuNES *cpu) {
  printf ("cpu_nes_exec_sei\n");
  cpu_nes_set_interrupt_disable_flag (cpu, TRUE);
}

void
cpu_nes_exec_sta (CpuNES *cpu) {
  ADDR16 addr;

  printf ("cpu_nes_exec_sta\n");

  addr = ((rom_nes_get_prg_memory (cpu->rom, cpu->reg_ip.r_16 + 1) << 8) | rom_nes_get_prg_memory (cpu->rom, cpu->reg_ip.r_16 + 2));
  cpu_nes_set_memory (cpu, addr, cpu->reg_a);
}

const struct {
  char *name;
  unsigned int cycle;
  CpuNESExecOp func;
  unsigned int bytes;
  AddressingMode mode;
} OpCodes[256] = {
  /* */
  { "BRK", 7, cpu_nes_exec_brk, 1, ADDR_MODE_IMPLIED }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "SEI", 2, cpu_nes_exec_sei, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "STA", 2, cpu_nes_exec_sta, 3 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "LDX", 2, cpu_nes_exec_ldx, 2 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "LDA", 2, cpu_nes_exec_lda, 2 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "CLD", 2, cpu_nes_exec_cld, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
  { "-", 1, cpu_nes_exec_null, 1 }, /* */
};

void
cpu_nes_step (CpuNES *cpu) {
  INT8 opcode;

  opcode = rom_nes_get_prg_memory (cpu->rom, cpu->reg_ip.r_16);

  printf ("\t%s : [opcode] %02x\n", FUNC, opcode & 0xff);
  if ((opcode & 0xff) < 0xff) {
    printf ("\t\t");
    OpCodes[opcode & 0xff].func (cpu);
    cpu->reg_ip.r_16 += OpCodes[opcode & 0xff].bytes;
  }
}
