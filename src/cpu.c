#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

typedef void (*CPUExecOp) (CPU *cpu);

struct _CPU {
  REG8 reg_a;
  REG8 reg_x;
  REG8 reg_y;
  REG8 reg_status;
  REG8 reg_sp; // $0100-$01FF
  REG16 reg_ip;
  BYTE ram[0x0800]; // 2KB
  Rom *rom;
  Mapper *mapper;
};

CPU *
cpu_new (void) {
  CPU *cpu;

  cpu = calloc (sizeof (CPU), 1);

  cpu->reg_sp = 0xFF;

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
  cpu_set_memory (cpu, 0x0100 | (cpu->reg_sp & 0xff), value);
  cpu->reg_sp--;
}

BYTE
cpu_pop (CPU *cpu) {
  return cpu_get_memory (cpu, 0x0100 | (cpu->reg_sp & 0xff));
  cpu->reg_sp++;
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
  //printf ("a%04xa\n", (cpu_get_memory (cpu, 0xFFFE) | (cpu_get_memory (cpu, 0xFFFF) << 8)) & 0xFFFF);
  //cpu->reg_ip = cpu_get_memory (cpu, 0xFFFE) | (cpu_get_memory (cpu, 0xFFFF) << 8);
}

void
cpu_exec_cld (CPU *cpu) {
  printf ("cpu_exec_cld\n");
  cpu_set_decimal_flag (cpu, FALSE);
}

void
cpu_exec_lda (CPU *cpu) {
  printf ("cpu_exec_lda %02x\n", rom_get_prg_memory (cpu->rom, cpu->reg_ip + 1));
  cpu->reg_a = rom_get_prg_memory (cpu->rom, cpu->reg_ip + 1);
  cpu_set_zero_flag (cpu, cpu->reg_a == 0 ? TRUE : FALSE);
  cpu_set_negative_flag (cpu, cpu->reg_a < 0 ? TRUE : FALSE);
}

void
cpu_exec_ldx (CPU *cpu) {
  printf ("cpu_exec_ldx\n");

  cpu->reg_x = rom_get_prg_memory (cpu->rom, cpu->reg_ip + 1);
  cpu_set_zero_flag (cpu, cpu->reg_x == 0 ? TRUE : FALSE);
  cpu_set_negative_flag (cpu, cpu->reg_x < 0 ? TRUE : FALSE);
}

void
cpu_exec_sei (CPU *cpu) {
  printf ("cpu_exec_sei\n");
  cpu_set_interrupt_disable_flag (cpu, TRUE);
}

void
cpu_exec_sta (CPU *cpu) {
  printf ("cpu_exec_sta\n");
  ADDR16 addr;

  addr = ((rom_get_prg_memory (cpu->rom, cpu->reg_ip + 1) << 8) & 0xFF) | (rom_get_prg_memory (cpu->rom, cpu->reg_ip + 2) & 0xFF);
  cpu_set_memory (cpu, addr, cpu->reg_a);
}

const struct {
  char *name;
  unsigned int cycle;
  CPUExecOp func;
  unsigned int bytes;
} OpCodes[256] = {
  // 0x00 - 0x1F OK
  { "BRK", 7, cpu_exec_brk, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x10
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x20
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x30
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x40
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x50
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x60
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x70
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "SEI", 2, cpu_exec_sei, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x80
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "STA", 2, cpu_exec_sta, 3 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0x90
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0xA0
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "LDX", 2, cpu_exec_ldx, 2 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "LDA", 2, cpu_exec_lda, 2 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0xB0
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0xC0
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0xD0
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "CLD", 2, cpu_exec_cld, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0xE0
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
  // 0xF0
  { "-", 1, cpu_exec_null, 1 }, // 0x00
  { "-", 1, cpu_exec_null, 1 }, // 0x01
  { "-", 1, cpu_exec_null, 1 }, // 0x02
  { "-", 1, cpu_exec_null, 1 }, // 0x03
  { "-", 1, cpu_exec_null, 1 }, // 0x04
  { "-", 1, cpu_exec_null, 1 }, // 0x05
  { "-", 1, cpu_exec_null, 1 }, // 0x06
  { "-", 1, cpu_exec_null, 1 }, // 0x07
  { "-", 1, cpu_exec_null, 1 }, // 0x08
  { "-", 1, cpu_exec_null, 1 }, // 0x09
  { "-", 1, cpu_exec_null, 1 }, // 0x0A
  { "-", 1, cpu_exec_null, 1 }, // 0x0B
  { "-", 1, cpu_exec_null, 1 }, // 0x0C
  { "-", 1, cpu_exec_null, 1 }, // 0x0D
  { "-", 1, cpu_exec_null, 1 }, // 0x0E
  { "-", 1, cpu_exec_null, 1 }, // 0x0F
};

void
cpu_step (CPU *cpu) {
  INT8 opcode;

  opcode = rom_get_prg_memory (cpu->rom, cpu->reg_ip);

  printf ("\t%s : [opcode] %02x\n", FUNC, opcode & 0xff);
  if ((opcode & 0xff) < 0xff) {
    printf ("\t\t");
    OpCodes[opcode & 0xff].func (cpu);
    cpu->reg_ip += OpCodes[opcode & 0xff].bytes;
  }
}
