#include "cpu_gb.h"

#include "cpu.h"
#include "map_gb.h"
#include "utils.h"
#include "cpu_op_16b.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
  //unsigned int : 4;
  unsigned int negative_flag;
  unsigned int half_carry_flag;
  unsigned int carry_flag;
  unsigned int zero_flag;
} CpuGBFlags;

struct _CpuGB {
  Cpu cpu;

  REG16 AF; /* r_8.h = A */
  REG16 BC; /* r_8.h = B | r_8.l = C */
  REG16 DE; /* r_8.h = D | r_8.l = E */
  REG16 HL; /* r_8.h = H | r_8.l = L */
  REG16 SP;
  REG16 PC;

  BYTE rom[0x0100]; /* 256B */
  BYTE ram[0x2000]; /* 8KB */
  BYTE hram[0X0080]; /* 128B */

  CpuGBFlags *flags;

  MapGB *map;

  UINT8 cycles;

  BOOL IME;
};

void cpu_gb_free (Cpu *cpu);
UINT8 cpu_gb_step (Cpu *cpu);

CpuGB *
cpu_gb_new  (void) {
  CpuGB *cpu;
  int rom_fd;

  cpu = calloc (sizeof (CpuGB), 1);

  //cpu->flags = (CpuGBFlags *) &cpu->AF.r_8.l;
  cpu->flags = calloc (sizeof (CpuGBFlags), 1);

  rom_fd = open ("../data/DMG_ROM.bin", O_RDONLY);

  if (rom_fd == -1) {
    perror ("cpu_gb_new");
    return NULL;
  }

  if (read (rom_fd, cpu->rom, 0x0100) == 0) {
    perror ("cpu_gb_new rom read");
    close (rom_fd);
    return NULL;
  }

  cpu->cpu.step = cpu_gb_step;
  cpu->cpu.free = cpu_gb_free;

  return cpu;
}

void
cpu_gb_free (Cpu *cpu) {
  free (cpu);
}

BYTE
cpu_gb_get_ram_memory (CpuGB *cpu, ADDR16 addr) {
  if (addr < 0x2000)
    return cpu->ram[addr];
  else {
    printf ("%s : address [%04x] overflow!\n", FUNC, addr);
    return 0;
  }
}

void
cpu_gb_set_ram_memory (CpuGB *cpu, ADDR16 addr, BYTE value) {
  if (addr < 0x2000)
    cpu->ram[addr] = value;
  else
    printf ("%s : address [%04x] overflow!\n", FUNC, addr);
}

BYTE
cpu_gb_get_hram_memory (CpuGB *cpu, ADDR16 addr) {
  if (addr < 0x0080)
    return cpu->hram[addr];
  else {
    printf ("%s : address [%04x] overflow!\n", FUNC, addr);
    return 0;
  }
}

void
cpu_gb_set_hram_memory (CpuGB *cpu, ADDR16 addr, BYTE value) {
  if (addr < 0x0080)
    cpu->hram[addr] = value;
  else
    printf ("%s : address [%04x] overflow!\n", FUNC, addr);
}

BYTE
cpu_gb_get_rom_memory (CpuGB *cpu, ADDR16 addr) {
  if (addr < 0x0100)
    return cpu->rom[addr];
  else {
    printf ("%s : address [%04x] overflow!\n", FUNC, addr);
    return 0;
  }
}

void
cpu_gb_set_mapper (CpuGB *cpu, MapGB *map) {
  cpu->map = map;
}

void
cpu_gb_update (CpuGB *cpu, UINT8 cycles, UINT8 bytes) {
  cpu->cycles += cycles;
  cpu->PC.r_16 += bytes;
}

UINT8
cpu_gb_step (Cpu *cpu_parent) {
  CpuGB *cpu = (CpuGB *) cpu_parent;
  UINT16 opcode;
  char *assembly = NULL;
  ADDR16 addr = cpu->PC.r_16;

  opcode = map_gb_get_memory (cpu->map, cpu->PC.r_16);

  switch (opcode) {
    case 0x00:
      assembly = "NOP";
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== LD reg, # ========== */
    case 0x06:
      assembly = "LD B, #";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x0E:
      assembly = "LD C, #";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x16:
      assembly = "LD D, #";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x1E:
      assembly = "LD E, #";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x26:
      assembly = "LD H, #";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x2E:
      assembly = "LD L, #";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== LD reg, ## ========== */
    case 0x01:
      assembly = "LD BC, ##";
      cpu_op_16b_ld (&(cpu->BC), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2));
      cpu_gb_update (cpu, 12, 3);
      break;
    case 0x11:
      assembly = "LD DE, ##";
      cpu_op_16b_ld (&(cpu->DE), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2));
      cpu_gb_update (cpu, 12, 3);
      break;
    case 0x21:
      assembly = "LD HL, ##";
      cpu_op_16b_ld (&(cpu->HL), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2));
      cpu_gb_update (cpu, 12, 3);
      break;
    case 0x31:
      assembly = "LD SP, ##";
      cpu_op_16b_ld (&(cpu->SP), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2));
      cpu_gb_update (cpu, 12, 3);
      break;
    /* ========== LD reg, reg ========== */
    case 0x0A:
      assembly = "LD A, (BC)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->BC.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x1A:
      assembly = "LD A, (DE)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->DE.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x40:
      assembly = "LD B, B";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x41:
      assembly = "LD B, C";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x42:
      assembly = "LD B, D";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x43:
      assembly = "LD B, E";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x44:
      assembly = "LD B, H";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x45:
      assembly = "LD B, L";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x46:
      assembly = "LD B, (HL)";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x47:
      assembly = "LD B, A";
      cpu_op_8b_ld (&(cpu->BC.r_8.h), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x48:
      assembly = "LD C, B";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x49:
      assembly = "LD C, C";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x4A:
      assembly = "LD C, D";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x4B:
      assembly = "LD C, E";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x4C:
      assembly = "LD C, H";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x4D:
      assembly = "LD C, L";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x4E:
      assembly = "LD C, (HL)";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x4F:
      assembly = "LD C, A";
      cpu_op_8b_ld (&(cpu->BC.r_8.l), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x50:
      assembly = "LD D, B";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x51:
      assembly = "LD D, C";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x52:
      assembly = "LD D, D";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x53:
      assembly = "LD D, E";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x54:
      assembly = "LD D, H";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x55:
      assembly = "LD D, L";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x56:
      assembly = "LD D, (HL)";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x57:
      assembly = "LD D, A";
      cpu_op_8b_ld (&(cpu->DE.r_8.h), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x58:
      assembly = "LD E, B";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x59:
      assembly = "LD E, C";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x5A:
      assembly = "LD E, D";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x5B:
      assembly = "LD E, E";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x5C:
      assembly = "LD E, H";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x5D:
      assembly = "LD E, L";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x5E:
      assembly = "LD E, (HL)";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x5F:
      assembly = "LD E, A";
      cpu_op_8b_ld (&(cpu->DE.r_8.l), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x60:
      assembly = "LD H, B";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x61:
      assembly = "LD H, C";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x62:
      assembly = "LD H, D";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x63:
      assembly = "LD H, E";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x64:
      assembly = "LD H, H";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x65:
      assembly = "LD H, L";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x66:
      assembly = "LD H, (HL)";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x67:
      assembly = "LD H, A";
      cpu_op_8b_ld (&(cpu->HL.r_8.h), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x68:
      assembly = "LD L, B";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x69:
      assembly = "LD L, C";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x6A:
      assembly = "LD L, D";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x6B:
      assembly = "LD L, E";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x6C:
      assembly = "LD L, H";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x6D:
      assembly = "LD L, L";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x6E:
      assembly = "LD L, (HL)";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x6F:
      assembly = "LD L, A";
      cpu_op_8b_ld (&(cpu->HL.r_8.l), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x70:
      assembly = "LD (HL), B";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->BC.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x71:
      assembly = "LD (HL), C";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->BC.r_8.l);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x72:
      assembly = "LD (HL), D";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->DE.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x73:
      assembly = "LD (HL), E";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->DE.r_8.l);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x74:
      assembly = "LD (HL), H";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->HL.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x75:
      assembly = "LD (HL), L";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->HL.r_8.l);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x77:
      assembly = "LD (HL), A";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->AF.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x78:
      assembly = "LD A, B";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->BC.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x79:
      assembly = "LD A, C";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->BC.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x7A:
      assembly = "LD A, D";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->DE.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x7B:
      assembly = "LD A, E";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->DE.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x7C:
      assembly = "LD A, H";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->HL.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x7D:
      assembly = "LD A, L";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->HL.r_8.l);
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x7E:
      assembly = "LD A, (HL)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x7F:
      assembly = "LD A, A";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 4, 1);
      break;

    /* ========== LD A, n ========== */
    case 0x3E:
      assembly = "LD A, #";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0xFA:
      assembly = "LD A, (##)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1)));
      cpu_gb_update (cpu, 16, 3);
      break;
    /* ========== LD n, A ========== */
    case 0x02:
      assembly = "LD (BC), A";
      map_gb_set_memory (cpu->map, cpu->BC.r_16, cpu->AF.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x12:
      assembly = "LD (DE), A";
      map_gb_set_memory (cpu->map, cpu->DE.r_16, cpu->AF.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0xEA:
      assembly = "LD (##), A";
      map_gb_set_memory (cpu->map, (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 16, 3);
      break;
    /* ========== LD (C), A ========== */
    case 0xE2:
      assembly = "LD (0xFF00 + C), A";
      map_gb_set_memory (cpu->map, 0xFF00 | cpu->BC.r_8.l, cpu->AF.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== LDD A, (HL) ========== */
    case 0x3A:
      assembly = "LDD A, (HL)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_op_16b_dec (&(cpu->HL));
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== LDD (HL), A ========== */
    case 0x32:
      assembly = "LDD (HL), A";
      map_gb_set_memory (cpu->map, cpu->HL.r_16--, cpu->AF.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== LDI A, (HL) ========== */
    case 0x2A:
      assembly = "LDI A, (HL)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_op_16b_inc (&(cpu->HL));
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== LDI (HL), A ========== */
    case 0x22:
      assembly = "LDI (HL), A";
      map_gb_set_memory (cpu->map, cpu->HL.r_16++, cpu->AF.r_8.h);
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== LDH (n), A ========== */
    case 0xE0:
      assembly = "LDH (0xFF00 + #), A";
      map_gb_set_memory (cpu->map, 0xFF00 | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), cpu->AF.r_8.h);
      cpu_gb_update (cpu, 12, 2);
      break;
    /* ========== LDH A, (n) ========== */
    case 0xF0:
      assembly = "LDH A, (0xFF00 + #)";
      cpu_op_8b_ld (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, 0xFF00 + map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1)));
      cpu_gb_update (cpu, 12, 2);
      break;
    /* ========== PUSH ========== */
    case 0xC5:
      assembly = "PUSH BC";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->BC.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->BC.r_8.l);
      cpu_gb_update (cpu, 16, 1);
      break;
    case 0xD5:
      assembly = "PUSH DE";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->DE.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->DE.r_8.l);
      cpu_gb_update (cpu, 16, 1);
      break;
    case 0xE5:
      assembly = "PUSH HL";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->HL.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->HL.r_8.l);
      cpu_gb_update (cpu, 16, 1);
      break;
    case 0xF5:
      assembly = "PUSH AF";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->AF.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->AF.r_8.l);
      cpu_gb_update (cpu, 16, 1);
      break;
    /* ========== POP ========== */
    case 0xC1:
      assembly = "POP BC";
      cpu->BC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->BC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu_gb_update (cpu, 12, 1);
      break;
    case 0xD1:
      assembly = "POP DE";
      cpu->DE.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->DE.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu_gb_update (cpu, 12, 1);
      break;
    case 0xE1:
      assembly = "POP HL";
      cpu->HL.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->HL.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu_gb_update (cpu, 12, 1);
      break;
    case 0xF1:
      assembly = "POP AF";
      cpu->AF.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu_gb_update (cpu, 12, 1);
      break;
    /* ========== ADD ========== */
    case 0x09:
      assembly = "ADD HL, BC";
      cpu_op_16b_add (&(cpu->HL), cpu->BC.r_16, &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x19:
      assembly = "ADD HL, DE";
      cpu_op_16b_add (&(cpu->HL), cpu->DE.r_16, &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x29:
      assembly = "ADD HL, HL";
      cpu_op_16b_add (&(cpu->HL), cpu->HL.r_16, &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x39:
      assembly = "ADD HL, SP";
      cpu_op_16b_add (&(cpu->HL), cpu->SP.r_16, &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0xC6:
      assembly = "ADD #";
      cpu_op_8b_add (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x80:
      assembly = "ADD B";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x81:
      assembly = "ADD C";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x82:
      assembly = "ADD D";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x83:
      assembly = "ADD E";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x84:
      assembly = "ADD H";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x85:
      assembly = "ADD L";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x86:
      assembly = "ADD (HL)";
      cpu_op_8b_add (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x87:
      assembly = "ADD A";
      cpu_op_8b_add (&(cpu->AF.r_8.h), cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xE8:
      assembly = "ADD SP, #";
      cpu_op_16b_add (&(cpu->SP), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 16, 2);
      break;
    /* ========== ADC ========== */
    case 0xCE:
      assembly = "ADC #";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x88:
      assembly = "ADC B";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x89:
      assembly = "ADC C";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x8A:
      assembly = "ADC D";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x8B:
      assembly = "ADC E";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x8C:
      assembly = "ADC H";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x8D:
      assembly = "ADC L";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x8E:
      assembly = "ADC (HL)";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x8F:
      assembly = "ADC A";
      cpu_op_8b_adc (&(cpu->AF.r_8.h), cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== SUB ========== */
    case 0xD6:
      assembly = "SUB #";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x90:
      assembly = "SUB B";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x91:
      assembly = "SUB C";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x92:
      assembly = "SUB D";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x93:
      assembly = "SUB E";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x94:
      assembly = "SUB H";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x95:
      assembly = "SUB L";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x96:
      assembly = "SUB (HL)";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x97:
      assembly = "SUB A";
      cpu_op_8b_sub (&(cpu->AF.r_8.h), cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== SBC ========== */
    /* ========== AND ========== */
    case 0xA0:
      assembly = "AND B";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA1:
      assembly = "AND C";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA2:
      assembly = "AND D";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA3:
      assembly = "AND E";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA4:
      assembly = "AND H";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA5:
      assembly = "AND L";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA6:
      assembly = "AND (HL)";
      cpu_op_8b_and (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0xA7:
      assembly = "AND A";
      cpu_op_8b_and (&(cpu->AF.r_8.h), cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xE6:
      assembly = "AND #";
      cpu_op_8b_and (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== OR ========== */
    case 0xB0:
      assembly = "OR B";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB1:
      assembly = "OR C";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB2:
      assembly = "OR D";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB3:
      assembly = "OR E";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB4:
      assembly = "OR H";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB5:
      assembly = "OR L";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB6:
      assembly = "OR (HL)";
      cpu_op_8b_or (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0xB7:
      assembly = "OR A";
      cpu_op_8b_or (&(cpu->AF.r_8.h), cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xF6:
      assembly = "OR #";
      cpu_op_8b_or (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== XOR ========== */
    case 0xA8:
      assembly = "XOR B";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xA9:
      assembly = "XOR C";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xAA:
      assembly = "XOR D";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xAB:
      assembly = "XOR E";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xAC:
      assembly = "XOR H";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xAD:
      assembly = "XOR L";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xAE:
      assembly = "XOR (HL)";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0xAF:
      assembly = "XOR A";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xEE:
      assembly = "XOR #";
      cpu_op_8b_xor (&(cpu->AF.r_8.h), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== CP ========== */
    case 0xB8:
      assembly = "CP B";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->BC.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xB9:
      assembly = "CP C";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->BC.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xBA:
      assembly = "CP D";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->DE.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xBB:
      assembly = "CP E";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->DE.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xBC:
      assembly = "CP H";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->HL.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xBD:
      assembly = "CP L";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->HL.r_8.l, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xBE:
      assembly = "CP (HL)";
      cpu_op_8b_cmp (cpu->AF.r_8.h, map_gb_get_memory (cpu->map, cpu->HL.r_16), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0xBF:
      assembly = "CP A";
      cpu_op_8b_cmp (cpu->AF.r_8.h, cpu->AF.r_8.h, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0xFE:
      assembly = "CP #";
      cpu_op_8b_cmp (cpu->AF.r_8.h, map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== INC # ========== */
    case 0x04:
      assembly = "INC B";
      cpu_op_8b_inc (&(cpu->BC.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x0C:
      assembly = "INC C";
      cpu_op_8b_inc (&(cpu->BC.r_8.l), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x14:
      assembly = "INC D";
      cpu_op_8b_inc (&(cpu->DE.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x1C:
      assembly = "INC E";
      cpu_op_8b_inc (&(cpu->DE.r_8.l), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x24:
      assembly = "INC H";
      cpu_op_8b_inc (&(cpu->HL.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x2C:
      assembly = "INC L";
      cpu_op_8b_inc (&(cpu->HL.r_8.l), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x34:
      assembly = "INC (HL)";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, map_gb_get_memory (cpu->map, cpu->HL.r_16) + 1);
      cpu_gb_update (cpu, 12, 1);
      break;
    case 0x3C:
      assembly = "INC A";
      cpu_op_8b_inc (&(cpu->AF.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== INC ## ========== */
    case 0x03:
      assembly = "INC BC";
      cpu_op_16b_inc (&(cpu->BC));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x13:
      assembly = "INC DE";
      cpu_op_16b_inc (&(cpu->DE));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x23:
      assembly = "INC HL";
      cpu_op_16b_inc (&(cpu->HL));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x33:
      assembly = "INC SP";
      cpu_op_16b_inc (&(cpu->SP));
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== DEC # ========== */
    case 0x05:
      assembly = "DEC B";
      cpu_op_8b_dec (&(cpu->BC.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x0D:
      assembly = "DEC C";
      cpu_op_8b_dec (&(cpu->BC.r_8.l), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x15:
      assembly = "DEC D";
      cpu_op_8b_dec (&(cpu->DE.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x1D:
      assembly = "DEC E";
      cpu_op_8b_dec (&(cpu->DE.r_8.l), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x25:
      assembly = "DEC H";
      cpu_op_8b_dec (&(cpu->HL.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x2D:
      assembly = "DEC L";
      cpu_op_8b_dec (&(cpu->HL.r_8.l), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x35:
      assembly = "DEC (HL)";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, map_gb_get_memory (cpu->map, cpu->HL.r_16));
      cpu_gb_update (cpu, 12, 1);
      break;
    case 0x3D:
      assembly = "DEC A";
      cpu_op_8b_dec (&(cpu->AF.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->zero_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== DEC ## ========== */
    case 0x0B:
      assembly = "DEC BC";
      cpu_op_16b_dec (&(cpu->BC));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x1B:
      assembly = "DEC DE";
      cpu_op_16b_dec (&(cpu->DE));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x2B:
      assembly = "DEC HL";
      cpu_op_16b_dec (&(cpu->HL));
      cpu_gb_update (cpu, 8, 1);
      break;
    case 0x3B:
      assembly = "DEC SP";
      cpu_op_16b_dec (&(cpu->SP));
      cpu_gb_update (cpu, 8, 1);
      break;
    /* ========== RLA ========== */
    case 0x0F:
      assembly = "RRCA";
      cpu_op_8b_rrc (&(cpu->AF.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
    case 0x17:
      assembly = "RLA";
      cpu_op_8b_rol (&(cpu->AF.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;
      
    /* ========== CB ========== */
    case 0xCB:
      opcode = (opcode << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      switch (opcode & 0xFF) {
        case 0x11:
          assembly = "RL C";
          cpu_op_8b_rol (&(cpu->BC.r_8.l), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
      /* ========== SWAP ========== */
        case 0x30:
          assembly = "SWAP B";
          cpu_op_8b_swap (&(cpu->BC.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        case 0x31:
          assembly = "SWAP C";
          cpu_op_8b_swap (&(cpu->BC.r_8.l), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        case 0x32:
          assembly = "SWAP D";
          cpu_op_8b_swap (&(cpu->DE.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        case 0x33:
          assembly = "SWAP E";
          cpu_op_8b_swap (&(cpu->DE.r_8.l), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        case 0x34:
          assembly = "SWAP H";
          cpu_op_8b_swap (&(cpu->HL.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        case 0x35:
          assembly = "SWAP L";
          cpu_op_8b_swap (&(cpu->HL.r_8.l), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        /*case 0x36:
          assembly = "SWAP (HL)";
          cpu_op_8b_swap (&(cpu->BC.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 16, 2);
          break;*/
        case 0x37:
          assembly = "SWAP A";
          cpu_op_8b_swap (&(cpu->AF.r_8.h), &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag), &(cpu->flags->carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
      /* ========== BIT ========== */
        case 0x7C:
          assembly = "BIT 7, H";
          cpu_op_8b_bit (&(cpu->HL.r_8.h), 7, &(cpu->flags->zero_flag), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag));
          cpu_gb_update (cpu, 8, 2);
          break;
        case 0x87:
          assembly = "RES 0, A";
          cpu->AF.r_8.h &= 0xFE;
          cpu_gb_update (cpu, 8, 2);
          break;
        default:
          assembly = "!!! NOT IMPLEMENTED !!!";
          cpu_gb_update (cpu, 4, 0);
          break;
      }
      break;
    /* ========== JP ## ========== */
    case 0xC3:
      assembly = "JP ##";
      cpu_op_16b_jp (&(cpu->PC), (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 12, 0);
      break;
    /* ========== JP cc, ## ========== */
    case 0xC2:
      assembly = "JP NZ, ##";
      if (cpu->flags->zero_flag == FALSE) {
        cpu_op_16b_jp (&(cpu->PC), (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      else
        cpu_gb_update (cpu, 12, 3);
      break;
    case 0xCA:
      assembly = "JP Z, ##";
      if (cpu->flags->zero_flag == TRUE) {
        cpu_op_16b_jp (&(cpu->PC), (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      else
        cpu_gb_update (cpu, 12, 3);
      break;
    case 0xD2:
      assembly = "JP NC, ##";
      if (cpu->flags->carry_flag == FALSE) {
        cpu_op_16b_jp (&(cpu->PC), (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      else
        cpu_gb_update (cpu, 12, 3);
      break;
    case 0xDA:
      assembly = "JP C, ##";
      if (cpu->flags->carry_flag == TRUE) {
        cpu_op_16b_jp (&(cpu->PC), (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      else
        cpu_gb_update (cpu, 12, 3);
      break;
    /* ========== JP (HL) ========== */
    case 0xE9:
      assembly = "JP (HL)";
      cpu_op_16b_jp (&(cpu->PC), cpu->HL.r_16);
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== JR # ========== */
    case 0x18:
      assembly = "JR #";
      cpu_op_16b_jr (&(cpu->PC), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== JR cc, # ========== */
    case 0x20:
      assembly = "JR NZ, #";
      if (cpu->flags->zero_flag == FALSE) {
        cpu_op_16b_jr (&(cpu->PC), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x28:
      assembly = "JR Z, #";
      if (cpu->flags->zero_flag == TRUE) {
        cpu_op_16b_jr (&(cpu->PC), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x30:
      assembly = "JR NC, #";
      if (cpu->flags->carry_flag == FALSE) {
        cpu_op_16b_jr (&(cpu->PC), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      cpu_gb_update (cpu, 8, 2);
      break;
    case 0x38:
      assembly = "JR C, #";
      if (cpu->flags->carry_flag == TRUE) {
        cpu_op_16b_jr (&(cpu->PC), map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      }
      cpu_gb_update (cpu, 8, 2);
      break;
    /* ========== CALL ## ========== */
    case 0xCD:
      assembly = "CALL ##";
      REG16 tmp;
      tmp.r_16 = cpu->PC.r_16 + 3;
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
      cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8);
      cpu->cycles = 12;
      break;
    /* ========== CALL cc, ## ========== */
    case 0xC4:
      assembly = "CALL NZ, ##";
      if (cpu->flags->zero_flag == FALSE) {
        REG16 tmp;
        tmp.r_16 = cpu->PC.r_16 + 3;
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
        cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8);
        cpu_gb_update (cpu, 12, 0);
      }
      else {
        cpu_gb_update (cpu, 12, 3);
      }
      break;
    case 0xCC:
      assembly = "CALL Z, ##";
      if (cpu->flags->zero_flag == TRUE) {
        REG16 tmp;
        tmp.r_16 = cpu->PC.r_16 + 3;
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
        cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8);
        cpu_gb_update (cpu, 12, 0);
      }
      else {
        cpu_gb_update (cpu, 12, 3);
      }
      break;
    case 0xD4:
      assembly = "CALL NC, ##";
      if (cpu->flags->carry_flag == FALSE) {
        REG16 tmp;
        tmp.r_16 = cpu->PC.r_16 + 3;
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
        cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8);
        cpu_gb_update (cpu, 12, 0);
      }
      else {
        cpu_gb_update (cpu, 12, 3);
      }
      break;
    case 0xDC:
      assembly = "CALL C, ##";
      if (cpu->flags->carry_flag == TRUE) {
        REG16 tmp;
        tmp.r_16 = cpu->PC.r_16 + 3;
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
        map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
        cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8);
        cpu_gb_update (cpu, 12, 0);
      }
      else {
        cpu_gb_update (cpu, 12, 3);
      }
      break;
    /* ========== RET ========== */
    case 0xC9:
      assembly = "RET";
      cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu_gb_update (cpu, 8, 0);
      break;
    /* ========== RET cc ========== */
    case 0xC0:
      assembly = "RET NZ";
      if (cpu->flags->zero_flag == FALSE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu_gb_update (cpu, 8, 0);
      }
      else {
        cpu_gb_update (cpu, 8, 1);
      }
      break;
    case 0xC8:
      assembly = "RET Z";
      if (cpu->flags->zero_flag == TRUE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu_gb_update (cpu, 8, 0);
      }
      else {
        cpu_gb_update (cpu, 8, 1);
      }
      break;
    case 0xD0:
      assembly = "RET NC";
      if (cpu->flags->carry_flag == FALSE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu_gb_update (cpu, 8, 0);
      }
      else {
        cpu_gb_update (cpu, 8, 1);
      }
      break;
    case 0xD8:
      assembly = "RET C";
      if (cpu->flags->carry_flag == TRUE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu_gb_update (cpu, 8, 0);
      }
      else {
        cpu_gb_update (cpu, 8, 1);
      }
      break;
    /* ========== RETI ========== */
    case 0xD9:
      assembly = "RETI";
      cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->IME = TRUE;
      cpu_gb_update (cpu, 8, 0);
      break;
    /* ========== MISC ========== */
    /* ========== EI ========== */
    case 0xFB:
      assembly = "EI";
      cpu->IME = TRUE;
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== DI ========== */
    case 0xF3:
      assembly = "DI";
      cpu->IME = FALSE;
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== CCF ========== */
    case 0x3F:
      assembly = "CCF";
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = FALSE;
      cpu->flags->carry_flag += 1;
      cpu_gb_update (cpu, 4, 1);
      break;
    /* ========== CPL ========== */
    case 0x2F:
      assembly = "CPL";
      cpu_op_8b_cpl (&(cpu->AF.r_8.h), &(cpu->flags->negative_flag), &(cpu->flags->half_carry_flag));
      cpu_gb_update (cpu, 4, 1);
      break;

    default:
      assembly = "!!! NOT IMPLEMENTED !!!";
      cpu->cycles = 4;
      break;
  }

  printf_asm (addr, opcode, assembly, "AF = %04x | BC = %04x | DE = %04x | HL = %04x | SP = %04x | Z = %u | N = %u| H = %u | C = %u", cpu->AF.r_16, cpu->BC.r_16, cpu->DE.r_16, cpu->HL.r_16, cpu->SP.r_16, cpu->flags->zero_flag, cpu->flags->negative_flag, cpu->flags->half_carry_flag, cpu->flags->carry_flag);

  return cpu->cycles;
}

void
cpu_gb_interrupt (CpuGB *cpu) {
  BYTE interrupt_enable, interrupt_request;
  int i;

  if (cpu->IME == FALSE)
    return;

  interrupt_enable = map_gb_get_memory (cpu->map, 0xFFFF);
  interrupt_request = map_gb_get_memory (cpu->map, 0xFF0F);

  for (i = 0; i < 5; ++i) {
    if (interrupt_enable & interrupt_request & (0x01 << i)) {
      printf ("INTERRUPTION FOUND %d!!!!\n", i);
      map_gb_set_memory (cpu->map, 0xFF0F, interrupt_request ^ (0x01 << i));
      cpu->IME = FALSE;
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->PC.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->PC.r_8.l);
      cpu->PC.r_16 = 0x40 + (i * 8);
      return;
    }
  }
}

void
cpu_gb_update_clock (CpuGB *cpu, UINT32 cycles) {
  UINT8 tac, tima, tma;
  UINT32 tac_freq;

  /* CPU_GB_FREQ / 16384 = 256 */
  if ((cycles % 256) == 0) {
    /* FIXME : cycles < 256 -> cycles > 256 ... */
    /* FIXME : Writing any value to this register resets it to 00h. */
    map_gb_set_memory (cpu->map, 0xFF04, map_gb_get_memory (cpu->map, 0xFF04) + 1);
  }

  tac = map_gb_get_memory (cpu->map, 0xFF07);

  if ((tac & 0x04) == FALSE)
    return;

  if ((tac & 0x03) == 0)
    tac_freq = 4096;
  else if ((tac & 0x03) == 1)
    tac_freq = 262144;
  else if ((tac & 0x03) == 2)
    tac_freq = 65536;
  else /* if ((tac & 0x03) == 3) */
    tac_freq = 16384;

  if ((cycles % (CPU_GB_FREQ / tac_freq)) == 0) {
    tima = map_gb_get_memory (cpu->map, 0xFF05);
    tma = map_gb_get_memory (cpu->map, 0xFF06);
    if (tima == 0xFF) {
      map_gb_set_memory (cpu->map, 0xFF05, tma);
      map_gb_set_memory (cpu->map, 0xFF0F, 0x04);
    }
    else {
      map_gb_set_memory (cpu->map, 0xFF05, tima + 1);
    }
  }
}