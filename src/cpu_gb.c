#include "cpu_gb.h"

#include "types.h"
#include "map_gb.h"
#include <stdio.h>
#include <stdlib.h>

struct _CpuGB {
  REG16 AF; /* r_8.h = A */
  REG16 BC; /* r_8.h = B | r_8.l = C */
  REG16 DE; /* r_8.h = D | r_8.l = E */
  REG16 HL; /* r_8.h = H | r_8.l = L */
  REG16 SP;
  REG16 PC;

  BYTE ram[0x2000]; /* 8KB */

  UINT32 ticks;
};

CpuGB *
cpu_gb_new  (void) {
  CpuGB *cpu;

  cpu = calloc (sizeof (CpuGB), 1);

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
cpu_gb_step (CpuGB *cpu) {
  BYTE opcode = 0;

/*  opcode = cpu_gb_get_memory (cpu, cpu->SP); */

  switch (opcode) {
    case 0x00:
      cpu->SP.r_16++;
      cpu->ticks += 4;
      break;
    case 0x01:
      /*cpu->BC.r_16 = cpu_gb_get_memory (cpu, cpu->SP + 1) & (cpu_gb_get_memory (cpu, cpu->SP + 2) << 8);*/
      break;
    case 0x02:
      break;
    case 0x03:
      break;
    case 0x04:
      break;
    case 0x05:
      break;
    case 0x06:
      break;
    case 0x07:
      break;
    case 0x08:
      break;
    case 0x09:
      break;
    case 0x0A:
      break;
    case 0x0B:
      break;
    case 0x0C:
      break;
    case 0x0D:
      break;
    case 0x0E:
      break;
    case 0x0F:
      break;
  }
}
