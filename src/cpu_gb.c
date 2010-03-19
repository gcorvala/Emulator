#include "cpu_gb.h"

#include "map_gb.h"

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
  BYTE result;

  if (addr > 0x2000) {
    printf ("%s : address [%04x] overflow!\n", FUNC, addr);
    result = 0;
  }
  else
    result = cpu->ram[addr];

  return result;
}

void
cpu_gb_set_mapper (CpuGB *cpu, MapGB *map) {
  cpu->map = map;
}

void
cpu_gb_step (CpuGB *cpu) {
  BYTE opcode = 0;
  BOOL tmp;

  opcode = cpu->rom[cpu->PC.r_16];
/*  opcode = cpu_gb_get_memory (cpu, cpu->SP);
  printf ("\t%s : opcode [%02x]\n", FUNC, opcode);*/

  switch (opcode) {
    case 0x00:
      printf ("\t\tNOP\n");
      cpu->PC.r_16++;
      break;
    case 0x04:
      printf ("\t\tINC B\n");
      cpu->BC.r_8.h++;
      cpu->PC.r_16 += 1;
      break;
    case 0x05:
      printf ("\t\tDEC B\n");
      cpu->BC.r_8.h--;
      cpu->PC.r_16 += 1;
      break;
    case 0x06:
      printf ("\t\tLD B, %02x\n", cpu->rom[cpu->PC.r_16 + 1]);
      cpu->BC.r_8.h = cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0x0C:
      printf ("\t\tINC C\n");
      cpu->BC.r_8.l++;
      cpu->PC.r_16 += 1;
      break;
    case 0x0D:
      printf ("\t\tDEC C\n");
      cpu->BC.r_8.l--;
      cpu->PC.r_16 += 1;
      break;
    case 0x0E:
      printf ("\t\tLD C, %02x\n", cpu->rom[cpu->PC.r_16 + 1]);
      cpu->BC.r_8.l = cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0x11:
      cpu->DE.r_8.l = cpu->rom[cpu->PC.r_16 + 1];
      cpu->DE.r_8.h = cpu->rom[cpu->PC.r_16 + 2];
      printf ("\t\tLD DE, %04x\n", cpu->DE.r_16);
      cpu->PC.r_16 += 3;
      break;
    case 0x13:
      printf ("\t\tINC DE\n");
      cpu->DE.r_16++;
      cpu->PC.r_16 += 1;
      break;
    case 0x17:
      printf ("\t\tRL A\n");
      tmp = 0x80 & cpu->AF.r_8.h;
      cpu->AF.r_8.h <<= 1;
      if (cpu->flags->carry_flag)
        cpu->AF.r_8.h += 1;
      cpu->flags->negative_flag = FALSE;
      cpu->flags->half_carry_flag = FALSE;
      cpu->flags->carry_flag = tmp;
      if (cpu->AF.r_8.h == 0)
        cpu->flags->zero_flag = TRUE;
      cpu->PC.r_16 += 1;
      break;
    case 0x1A:
      printf ("\t\tLD A, (DE)\n");
      cpu->AF.r_8.h = map_gb_get_memory (cpu->map, cpu->DE.r_16);
      cpu->PC.r_16 += 1;
      break;
    case 0x1D:
      printf ("\t\tDEC E\n");
      cpu->DE.r_8.l--;
      cpu->PC.r_16 += 1;
      break;
    case 0x1E:
      printf ("\t\tLD E, %02x\n", cpu->rom[cpu->PC.r_16 + 1]);
      cpu->DE.r_8.l = cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0x20:
      printf ("\t\tJR NZ, %d [%04x]\n", (INT8) cpu->rom[cpu->PC.r_16 + 1], cpu->PC.r_16 + (INT8) cpu->rom[cpu->PC.r_16 + 1] + 2);
      if (cpu->flags->zero_flag == FALSE)
        cpu->PC.r_16 += (INT8) cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0x21:
      cpu->HL.r_8.l = cpu->rom[cpu->PC.r_16 + 1];
      cpu->HL.r_8.h = cpu->rom[cpu->PC.r_16 + 2];
      printf ("\t\tLD HL, %04x\n", cpu->HL.r_16);
      cpu->PC.r_16 += 3;
      break;
    case 0x22:
      printf ("\t\tLD (HL+), A\n");
      cpu->HL.r_16 = cpu->AF.r_8.h;
      cpu->HL.r_16++;
      cpu->PC.r_16 += 1;
      break;
    case 0x23:
      printf ("\t\tINC HL\n");
      cpu->HL.r_16++;
      cpu->PC.r_16 += 1;
      break;
    case 0x28:
      printf ("\t\tJR Z, %d [%04x]\n", (INT8) cpu->rom[cpu->PC.r_16 + 1], cpu->PC.r_16 + (INT8) cpu->rom[cpu->PC.r_16 + 1] + 2);
      if (cpu->flags->zero_flag == TRUE)
        cpu->PC.r_16 += (INT8) cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0x31:
      cpu->SP.r_8.l = cpu->rom[cpu->PC.r_16 + 1];
      cpu->SP.r_8.h = cpu->rom[cpu->PC.r_16 + 2];
      printf ("\t\tLD SP, %04x\n", cpu->SP.r_16);
      cpu->PC.r_16 += 3;
      break;
    case 0x32:
      printf ("\t\tLD (HL-), A\n");
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->AF.r_8.h);
      cpu->HL.r_16 -= 1;
      cpu->PC.r_16 += 1;
      break;
    case 0x3D:
      printf ("\t\tDEC A\n");
      cpu->AF.r_8.h--;
      cpu->PC.r_16 += 1;
      break;
    case 0x3E:
      printf ("\t\tLD A, %02x\n", cpu->rom[cpu->PC.r_16 + 1]);
      cpu->AF.r_8.h = cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0x4F:
      printf ("\t\tLD C, A\n");
      cpu->BC.r_8.l = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      break;
    case 0x57:
      printf ("\t\tLD D, A\n");
      cpu->DE.r_8.h = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      break;
    case 0x67:
      printf ("\t\tLD H, A\n");
      cpu->HL.r_8.h = cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      break;
    case 0x77:
      printf ("\t\tLD (HL), A\n");
      map_gb_set_memory (cpu->map, cpu->HL.r_16, cpu->AF.r_8.h);
      cpu->PC.r_16 += 1;
      break;
    case 0x7B:
      printf ("\t\tLD A, E\n");
      cpu->AF.r_8.h = cpu->DE.r_8.l;
      cpu->PC.r_16 += 1;
      break;
    case 0xAF:
      printf ("\t\tXOR A\n");
      cpu->AF.r_8.h ^= cpu->AF.r_8.h;
      cpu->PC.r_16 += 1;
      break;
    case 0xC1:
      printf ("\t\tPOP BC\n");
      cpu->BC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->BC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_16 += 1;
      break;
    case 0xC5:
      printf ("\t\tPUSH BC\n");
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->BC.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, cpu->BC.r_8.l);
      cpu->PC.r_16 += 1;
      break;
    case 0xC9:
      printf ("\t\tRET\n");
      cpu->PC.r_8.l = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      cpu->PC.r_8.h = map_gb_get_memory (cpu->map, ++cpu->SP.r_16);
      break;
    case 0xCB:
      opcode = cpu->rom[cpu->PC.r_16 + 1];
      switch (opcode) {
        case 0x11:
          printf ("\t\tRL C\n");
          tmp = 0x80 & cpu->BC.r_8.l;
          cpu->BC.r_8.l <<= 1;
          if (cpu->flags->carry_flag)
            cpu->BC.r_8.l += 1;
          cpu->flags->negative_flag = FALSE;
          cpu->flags->half_carry_flag = FALSE;
          cpu->flags->carry_flag = tmp;
          if (cpu->BC.r_8.l == 0)
            cpu->flags->zero_flag = TRUE;
          break;
        case 0x7C:
          printf ("\t\tBIT 7, H\n");
          cpu->flags->negative_flag = FALSE;
          cpu->flags->half_carry_flag = TRUE;
          cpu->flags->zero_flag = cpu->HL.r_8.h & 0x80 ? FALSE : TRUE;
          break;
        default:
          printf ("\t\tOPCODE[CB%02x] not implemented! [%04x]\n", opcode, cpu->PC.r_16);
          break;
      }
      cpu->PC.r_16 += 2;
      break;
    case 0xCD:
      printf ("\t\tCALL N\n");
      REG16 tmp;
      tmp.r_16 = cpu->PC.r_16 + 3;
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.h);
      map_gb_set_memory (cpu->map, cpu->SP.r_16--, tmp.r_8.l);
      cpu->PC.r_16 = cpu->rom[cpu->PC.r_16 + 1] | (cpu->rom[cpu->PC.r_16 + 2] << 8);
      break;
    case 0xE0:
      printf ("\t\tLD (FF00 + %02x), A\n", cpu->rom[cpu->PC.r_16 + 1]);
      map_gb_set_memory (cpu->map, 0xFF00 | cpu->rom[cpu->PC.r_16 + 1], cpu->AF.r_8.h);
      cpu->PC.r_16 += 2;
      break;
    case 0xE2:
      printf ("\t\tLD (FF00 + C), A\n");
      map_gb_set_memory (cpu->map, 0xFF00 | cpu->BC.r_8.l, cpu->AF.r_8.h);
      cpu->PC.r_16 += 1;
      break;
    case 0xEA:
      printf ("\t\tLD (%04x), A\n", cpu->rom[cpu->PC.r_16 + 1] | (cpu->rom[cpu->PC.r_16 + 2] << 8));
      map_gb_set_memory (cpu->map, cpu->rom[cpu->PC.r_16 + 1] | (cpu->rom[cpu->PC.r_16 + 2] << 8), cpu->AF.r_8.h);
      cpu->PC.r_16 += 3;
      break;
    case 0xF0:
      printf ("\t\tLD A, (FF00 + %02x)\n", cpu->rom[cpu->PC.r_16 + 1]);
      cpu->AF.r_8.h = 0xFF00 | cpu->rom[cpu->PC.r_16 + 1];
      cpu->PC.r_16 += 2;
      break;
    case 0xFE:
      printf ("\t\tCP %02x <- TODO set half_carry\n", cpu->rom[cpu->PC.r_16 + 1]);
      if (cpu->AF.r_8.h == cpu->rom[cpu->PC.r_16 + 1])
        cpu->flags->zero_flag = TRUE;
      cpu->flags->negative_flag = TRUE;
      if (cpu->AF.r_8.h < cpu->rom[cpu->PC.r_16 + 1])
        cpu->flags->carry_flag = TRUE;
      cpu->PC.r_16 += 2;
      break;
    default:
      printf ("\t\tOPCODE[%02x] not implemented! [%04x]\n", opcode, cpu->PC.r_16);
      break;
  }
}
