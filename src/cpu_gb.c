#include "cpu_gb.h"

#include "map_gb.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
  unsigned int unset : 4;
  unsigned int negative_flag : 1;
  unsigned int half_carry_flag : 1;
  unsigned int carry_flag : 1;
  unsigned int zero_flag : 1;
} CpuGBFlags;

struct _CpuGB {
  REG16 AF; /* r_8.h = A */
  REG16 BC; /* r_8.h = B | r_8.l = C */
  REG16 DE; /* r_8.h = D | r_8.l = E */
  REG16 HL; /* r_8.h = H | r_8.l = L */
  REG16 SP;
  REG16 PC;

  BYTE rom[0x0100]; /* 256B */
  BYTE ram[0x2000]; /* 8KB */
  BYTE hram[0X0080]; /* 128B */

  UINT32 ticks;

  CpuGBFlags *flags;

  MapGB *map;
};

CpuGB *
cpu_gb_new  (void) {
  CpuGB *cpu;
  int rom_fd;

  cpu = calloc (sizeof (CpuGB), 1);

  cpu->flags = (CpuGBFlags *) &cpu->AF.r_8.l;

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

  return cpu;
}

void
cpu_gb_free (CpuGB *cpu) {
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

UINT8
cpu_gb_step (CpuGB *cpu) {
  UINT16 opcode;
  BOOL tmp;
  UINT8 cycles = 0;
  char *assembly = NULL;
  ADDR16 addr = cpu->PC.r_16;

  opcode = map_gb_get_memory (cpu->map, cpu->PC.r_16);

  switch (opcode) {
    /* n h c z */
    case 0x00: /* OK */
      assembly = "NOP";
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x01:
      assembly = "LD BC, ##";
      cpu->BC.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->BC.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2);
      cpu->PC.r_16 += 3;
      cycles = 12;
      break;
    case 0x04: /* OK */
      assembly = "INC B";
      cpu->BC.r_8.h++;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->BC.r_8.h & 0x1F) == 0x10 ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->BC.r_8.h == 0x00 ? TRUE : FALSE; 
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x05: /* OK */
      assembly = "DEC B";
      cpu->BC.r_8.h--;
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = (cpu->BC.r_8.h & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->BC.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x06: /* OK */
      assembly = "LD B, #";
      cpu->BC.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x09:
      assembly = "ADD HL, BC";
      cpu->HL.r_16 += cpu->BC.r_16;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->HL.r_8.h & 0x0F) + (cpu->BC.r_8.h & 0x0F) > 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->HL.r_16 + cpu->BC.r_16 > 0xFFFF ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x0B:
      assembly = "DEC BC";
      cpu->BC.r_16--;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x0C: /* OK */
      assembly = "INC C";
      cpu->BC.r_8.l++;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->BC.r_8.l & 0x1F) == 0x10 ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->BC.r_8.l == 0x00 ? TRUE : FALSE; 
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x0D: /* OK */
      assembly = "DEC C";
      cpu->BC.r_8.l--;
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = (cpu->BC.r_8.l & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->BC.r_8.l == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x0E: /* OK */
      assembly = "LD C, #";
      cpu->BC.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x11: /* OK */
      assembly = "LD DE, ##";
      cpu->DE.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->DE.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2);
      cpu->PC.r_16 += 3;
      cycles = 12;
      break;
    case 0x12:
      assembly = "LD (DE), A";
      map_gb_set_memory (cpu->map, cpu->DE.r_16, cpu->AF.r_8.h);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x13: /* OK */
      assembly = "INC DE";
      cpu->DE.r_16++;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x15: /* OK */
      assembly = "DEC D";
      cpu->DE.r_8.h--;
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = (cpu->DE.r_8.h & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->DE.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x16: /* OK */
      assembly = "LD D, #";
      cpu->DE.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x17: /* OK */
      assembly = "RL A";
      tmp = 0x80 & cpu->AF.r_8.h;
      cpu->AF.r_8.h <<= 1;
      cpu->AF.r_8.h += cpu->flags->carry_flag ? 1 : 0;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = FALSE;
      cpu->flags->carry_flag = tmp ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x18: /* OK */
      assembly = "JR #";
      cpu->PC.r_16 += (INT8) map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) + 2;
      cycles = 8;
      break;
    case 0x19:
      assembly = "ADD HL, DE";
      cpu->HL.r_16 += cpu->DE.r_16;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->HL.r_8.h & 0x0F) + (cpu->DE.r_8.h & 0x0F) > 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->HL.r_16 + cpu->DE.r_16 > 0xFFFF ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x1A: /* OK */
      assembly = "LD A, (DE)";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, cpu->DE.r_16);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x1D: /* OK */
      assembly = "DEC E";
      cpu->DE.r_8.l--;
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = (cpu->DE.r_8.l & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->DE.r_8.l == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x1E: /* OK */
      assembly = "LD E, #";
      cpu->DE.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x20: /* OK */
      assembly = "JR NZ, #";
      cpu->PC.r_16 += cpu->flags->zero_flag ? 2 : (INT8) map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) + 2;
      cycles = 8;
      break;
    case 0x21: /* OK */
      assembly = "LD HL, ##";
      cpu->HL.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->HL.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2);
      cpu->PC.r_16 += 3;
      cycles = 12;
      break;
    case 0x22: /* OK */
      assembly = "LD (HL+), A";
      map_gb_set_memory (cpu->map, cpu->HL.r_16++, cpu->AF.r_8.h);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x23: /* OK */
      assembly = "INC HL";
      cpu->HL.r_16++;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x24: /* OK */
      assembly = "INC H";
      cpu->HL.r_8.h++;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->HL.r_8.h & 0x1F) == 0x10 ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->HL.r_8.h == 0x00 ? TRUE : FALSE; 
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x28: /* OK */
      assembly = "JR Z, #";
      cpu->PC.r_16 += cpu->flags->zero_flag ? (INT8) map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) + 2 : 2;
      cycles = 8;
      break;
    case 0x2A: /* OK */
      assembly = "LD A, (HL+)";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, cpu->HL.r_16++);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x2E: /* OK */
      assembly = "LD L, #";
      cpu->HL.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x30: /* OK */
      assembly = "JR NC, #";
      cpu->PC.r_16 += cpu->flags->carry_flag ? 2 : (INT8) map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) + 2;
      cycles = 8;
      break;
    case 0x31:
      assembly = "LD SP, ##";
      cpu->SP.r_8.l = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->SP.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2);
      cpu->PC.r_16 += 3;
      cycles = 12;
      break;
    case 0x32:
      assembly = "LD (HL-), A";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->AF.r_8.h);
      cpu->HL.r_16 -= 1;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x38: /* OK */
      assembly = "JR C, #";
      if (cpu->flags->carry_flag == TRUE)
        cpu->PC.r_16 += (INT8) map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x3A: /* OK */
      assembly = "LD A, (HL-)";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, cpu->HL.r_16--);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x3D:
      assembly = "DEC A";
      cpu->AF.r_8.h--;
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = (cpu->AF.r_8.h & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x3E:
      assembly = "LD A, #";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0x47: /* OK */
      assembly = "LD B, A";
      cpu->BC.r_8.h = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x4F: /* OK */
      assembly = "LD C, A";
      cpu->BC.r_8.l = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x54:
      assembly = "LD D, H";
      cpu->DE.r_8.h = cpu->HL.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x57: /* OK */
      assembly = "LD D, A";
      cpu->DE.r_8.h = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x5D:
      assembly = "LD E, L";
      cpu->DE.r_8.l = cpu->HL.r_8.l;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x5F: /* OK */
      assembly = "LD E, A";
      cpu->DE.r_8.l = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x66:
      assembly = "LD H, (HL)";
      cpu->HL.r_8.h = map_gb_get_memory (cpu->map, cpu->HL.r_16);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x67:
      assembly = "LD H, A";
      cpu->HL.r_8.h = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x68:
      assembly = "LD L, B";
      cpu->HL.r_8.l = cpu->BC.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x6F: /* OK */
      assembly = "LD L, A";
      cpu->HL.r_8.l = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x7C:
      assembly = "LD A, H";
      cpu->AF.r_8.h = cpu->HL.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x77:
      assembly = "LD (HL), A";
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->AF.r_8.h);
      cpu->PC.r_16 += 1;
      cycles= 8;
      break;
    case 0x78:
      assembly = "LD A, B";
      cpu->AF.r_8.h = cpu->BC.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x79: /* OK */
      assembly = "LD A, C";
      cpu->AF.r_8.h = cpu->BC.r_8.l;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x7A:
      assembly = "LD A, D";
      cpu->AF.r_8.h = cpu->DE.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x7B:
      assembly = "LD A, E";
      cpu->AF.r_8.h = cpu->DE.r_8.l;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x7D:
      assembly = "LD A, L";
      cpu->AF.r_8.h = cpu->HL.r_8.l;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x7E: /* OK */
      assembly = "LD A, (HL)";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, cpu->HL.r_16);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x85:
      assembly = "ADD A, L";
      cpu->AF.r_8.h += cpu->HL.r_8.l;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->AF.r_8.h & 0x0F) + (cpu->HL.r_8.l & 0x0F) > 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->AF.r_8.h + cpu->HL.r_8.l > 0xFF ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0x86:
      assembly = "ADD A, (HL)";
      cpu->AF.r_8.h += map_gb_get_memory (cpu->map, cpu->HL.r_16);
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->AF.r_8.h & 0x0F) + (map_gb_get_memory (cpu->map, cpu->HL.r_16) & 0x0F) > 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->AF.r_8.h + map_gb_get_memory (cpu->map, cpu->HL.r_16) > 0xFF ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0x90:
      /* FIXME */
      assembly = "SUB B";
      if (cpu->AF.r_8.h < cpu->BC.r_8.h)
        cpu->flags->carry_flag = TRUE;
      cpu->AF.r_8.h -= cpu->BC.r_8.h;
      if (cpu->AF.r_8.h == 0)
        cpu->flags->zero_flag = TRUE;
      cpu->flags->negative_flag = TRUE;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xA7: /* OK */
      assembly = "AND A";
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = TRUE;
      cpu->flags->carry_flag = FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h ? FALSE : TRUE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0xAF:
      assembly = "XOR A";
      cpu->AF.r_8.h ^= cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0xB0: /* OK */
      assembly = "OR B";
      cpu->AF.r_8.h = cpu->AF.r_8.h | cpu->BC.r_8.h;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = FALSE;
      cpu->flags->carry_flag = FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h ? FALSE : TRUE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0xB1: /* OK */
      assembly = "OR C";
      cpu->AF.r_8.h = cpu->AF.r_8.h | cpu->BC.r_8.l;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = FALSE;
      cpu->flags->carry_flag = FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h ? FALSE : TRUE;
      cpu->PC.r_16 += 1;
      cycles = 4;
      break;
    case 0xBA:
      assembly = "CP D";
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = ((cpu->AF.r_8.h - cpu->DE.r_8.h) & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->AF.r_8.h < cpu->DE.r_8.h ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == cpu->DE.r_8.h ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xBE:
      assembly = "CP (HL)";
      cpu->flags->negative_flag = TRUE;
      cpu->flags->half_carry_flag = ((cpu->AF.r_8.h - map_gb_get_memory (cpu->map, cpu->HL.r_16)) & 0x0F) == 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->AF.r_8.h < map_gb_get_memory (cpu->map, cpu->HL.r_16) ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == map_gb_get_memory (cpu->map, cpu->HL.r_16) ? TRUE : FALSE;
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xC0:
      assembly = "RET NZ";
      if (cpu->flags->zero_flag == FALSE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      }
      else
        cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xC1:
      assembly = "POP BC";
      cpu->BC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->BC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_16 += 1;
      cycles = 12;
      break;
    case 0xC3: /* OK */
      assembly = "JP ##";
      cpu->PC.r_16 = (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cycles = 12;
      break;
    case 0xC5:
      assembly = "PUSH BC";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->BC.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->BC.r_8.l);
      cpu->PC.r_16 += 1;
      cycles = 16;
      break;
    case 0xC6:
      assembly = "ADD A, #";
      cpu->AF.r_8.h += map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = (cpu->AF.r_8.h & 0x0F) + (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) & 0x0F) > 0x0F ? TRUE : FALSE;
      cpu->flags->carry_flag = cpu->AF.r_8.h + map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) > 0xFF ? TRUE : FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h == 0 ? TRUE : FALSE;
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0xC8:
      assembly = "RET Z";
      if (cpu->flags->zero_flag == TRUE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      }
      else
        cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xC9:
      assembly = "RET";
      cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cycles = 8;
      break;
    case 0xCA: /* OK */
      assembly = "JP Z, ##";
      if (cpu->flags->zero_flag == TRUE)
        cpu->PC.r_16 = (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      else
        cpu->PC.r_16 += 3;
      cycles = 12;
      break;
    case 0xCB:
      opcode = (opcode << 8) | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      switch (opcode & 0xFF) {
        case 0x11:
          assembly = "RL C";
          tmp = 0x80 & cpu->BC.r_8.l;
          cpu->BC.r_8.l <<= 1;
          if (cpu->flags->carry_flag)
            cpu->BC.r_8.l += 1;
          cpu->flags->negative_flag = FALSE;
          cpu->flags->half_carry_flag = FALSE;
          cpu->flags->carry_flag = tmp ? TRUE : FALSE;
          if (cpu->BC.r_8.l == 0)
            cpu->flags->zero_flag = TRUE;
          cycles = 8;
          break;
        case 0x27:
          assembly = "SLA A";
          cpu->flags->carry_flag = (cpu->AF.r_8.h & 0x80) >> 7;
          cpu->AF.r_8.h <<= 1;
          cpu->flags->zero_flag = cpu->AF.r_8.h ? FALSE : TRUE;
          cpu->flags->negative_flag = FALSE;
          cpu->flags->half_carry_flag = FALSE;
          cycles = 8;
          break;
        case 0x4F:
          assembly = "BIT 1, A";
          cpu->flags->negative_flag = FALSE;
          cpu->flags->half_carry_flag = TRUE;
          cpu->flags->zero_flag = cpu->AF.r_8.h & 0x02 ? FALSE : TRUE;
          cycles = 8; /* TO CHECK */
          break;
        case 0x7C:
          assembly = "BIT 7, H";
          cpu->flags->negative_flag = FALSE;
          cpu->flags->half_carry_flag = TRUE;
          cpu->flags->zero_flag = cpu->HL.r_8.h & 0x80 ? FALSE : TRUE;
          cycles = 8; /* TO CHECK */
          break;
        case 0x80: /* OK */
          assembly = "RES 0, B";
          cpu->BC.r_8.h &= 0xFE;
          cycles = 8;
          break;
        case 0x81: /* OK */
          assembly = "RES 0, C";
          cpu->BC.r_8.l &= 0xFE;
          cycles = 8;
          break;
        case 0x82: /* OK */
          assembly = "RES 0, D";
          cpu->DE.r_8.h &= 0xFE;
          cycles = 8;
          break;
        case 0x83: /* OK */
          assembly = "RES 0, E";
          cpu->DE.r_8.l &= 0xFE;
          cycles = 8;
          break;
        case 0x84: /* OK */
          assembly = "RES 0, H";
          cpu->HL.r_8.h &= 0xFE;
          cycles = 8;
          break;
        case 0x85: /* OK */
          assembly = "RES 0, L";
          cpu->HL.r_8.l &= 0xFE;
          cycles = 8;
          break;
        case 0x86: /* OK */
          assembly = "RES 0, (HL)";
          map_gb_set_memory (cpu->map, cpu->HL.r_16, map_gb_get_memory (cpu->map, cpu->HL.r_16) & 0xFE);
          cycles = 8;
          break;
        case 0x87: /* OK */
          assembly = "RES 0, A";
          cpu->AF.r_8.h &= 0xFE;
          cycles = 8;
          break;
        case 0xDE:
          assembly = "SET 3, (HL)";
          map_gb_set_memory (cpu->map, cpu->HL.r_16, map_gb_get_memory (cpu->map, cpu->HL.r_16) | 0x08);
          cycles = 16;
          break;
        default:
          assembly = "!!! NOT IMPLEMENTED !!!";
          cycles = 4;
          break;
      }
      cpu->PC.r_16 += 2;
      break;
    case 0xCD:
      assembly = "CALL ##";
      REG16 tmp;
      tmp.r_16 = cpu->PC.r_16 + 3;
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
      cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8);
      cycles = 12;
      break;
    case 0xCE:
      assembly = "ADC A, #";
      cpu->AF.r_8.h += map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) + cpu->flags->carry_flag;
      /* FIXME : set flags */
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0xD0:
      assembly = "RET NC";
      if (cpu->flags->carry_flag == FALSE) {
        cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
        cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      }
      else
        cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xD1:
      assembly = "POP DE";
      cpu->DE.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->DE.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_16 += 1;
      cycles = 12;
      break;
    case 0xD5:
      assembly = "PUSH DE";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->DE.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->DE.r_8.l);
      cpu->PC.r_16 += 1;
      cycles = 16;
      break;
    case 0xD6:
      assembly = "SUB #";
      /* FIXME half carry */
      if (cpu->AF.r_8.h < map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1))
        cpu->flags->carry_flag = TRUE;
      cpu->AF.r_8.h -= map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      if (cpu->AF.r_8.h == 0)
        cpu->flags->zero_flag = TRUE;
      cpu->flags->negative_flag = TRUE;
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0xE0:
      assembly = "LD (FF00 + #), A";
      map_gb_set_memory (cpu->map, 0xFF00 | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1), cpu->AF.r_8.h);
      cpu->PC.r_16 += 2;
      cycles = 12;
      break;
    case 0xE1: /* OK */
      assembly = "POP HL";
      cpu->HL.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->HL.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_16 += 1;
      cycles = 12;
      break;
    case 0xE2:
      assembly = "LD (FF00 + C), A";
      map_gb_set_memory (cpu->map, 0xFF00 | cpu->BC.r_8.l, cpu->AF.r_8.h);
      cpu->PC.r_16 += 1;
      cycles = 8;
      break;
    case 0xE5: /* OK */
      assembly = "PUSH HL";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->HL.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->HL.r_8.l);
      cpu->PC.r_16 += 1;
      cycles = 16;
      break;
    case 0xE6: /* OK */
      assembly = "AND #";
      cpu->AF.r_8.h &= map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1);
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = TRUE;
      cpu->flags->carry_flag = FALSE;
      cpu->flags->zero_flag = cpu->AF.r_8.h ? FALSE : TRUE;
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0xE9: /* OK */
      assembly = "JP (HL)";
      cpu->PC.r_16 = map_gb_get_memory (cpu->map, cpu->HL.r_16);
      cycles = 4;
      break;
    case 0xEA:
      assembly = "LD (##), A";
      map_gb_set_memory (cpu->map, map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8), cpu->AF.r_8.h);
      cpu->PC.r_16 += 3;
      cycles = 16;
      break;
    case 0xF0:
      assembly = "LD A, (FF00 + #)";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, 0xFF00 | map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1));
      cpu->PC.r_16 += 2;
      cycles = 12;
      break;
    case 0xF1:
      assembly = "POP AF";
      cpu->AF.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_16 += 1;
      cycles = 12;
      break;
    case 0xF3:
      /* FIXME */
      assembly = "DI";
      cpu->PC.r_16 += 1;
      break;
    case 0xF5:
      assembly = "PUSH AF";
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->AF.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->AF.r_8.l);
      cpu->PC.r_16 += 1;
      cycles = 16;
      break;
    case 0xFA:
      assembly = "LD A, (##)";
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) | (map_gb_get_memory (cpu->map, cpu->PC.r_16 + 2) << 8)));
      cpu->PC.r_16 += 3;
      cycles = 16;
      break;
    case 0xFB:
      /* FIXME */
      assembly = "EI";
      cpu->PC.r_16 += 1;
      break;
    case 0xFE:
      assembly = "CP #";
      cpu->flags->zero_flag = cpu->AF.r_8.h == map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1) ? TRUE : FALSE;
      cpu->flags->negative_flag = TRUE;
      if (cpu->AF.r_8.h < map_gb_get_memory (cpu->map, cpu->PC.r_16 + 1))
        cpu->flags->carry_flag = TRUE;
      cpu->PC.r_16 += 2;
      cycles = 8;
      break;
    case 0xFF:
      assembly = "RST $38";
      tmp.r_16 = cpu->PC.r_16 + 1;
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
      cpu->PC.r_16 = 0x0038;
      cycles = 32;
      break;
    default:
      assembly = "!!! NOT IMPLEMENTED !!!";
      cycles = 4;
      break;
  }

  printf_asm (addr, opcode, assembly, "AF = %04x | BC = %04x | DE = %04x | HL = %04x | Z = %u | N = %u| H = %u | C = %u", cpu->AF.r_16, cpu->BC.r_16, cpu->DE.r_16, cpu->HL.r_16, cpu->flags->zero_flag, cpu->flags->negative_flag, cpu->flags->half_carry_flag, cpu->flags->carry_flag);

  return cycles;
}
