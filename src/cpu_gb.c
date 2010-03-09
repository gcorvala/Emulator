#include "cpu_gb.h"

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

struct _CpuGB {
  REG16 AF; /* r_8.h = A */
  REG16 BC; /* r_8.h = B | r_8.l = C */
  REG16 DE; /* r_8.h = D | r_8.l = E */
  REG16 HL; /* r_8.h = H | r_8.l = L */
  REG16 SP;
  REG16 PC;
};

CpuGB *
cpu_gb_new  (void) {
  CpuGB *cpu;

  cpu = calloc (sizeof (CpuGB), 1);

  cpu->AF.r_16 = 0xFB84;
  printf ("%s : AF = %04x\n", FUNC, cpu->AF.r_16);
  printf ("%s : A = %02x\n", FUNC, cpu->AF.r_8.h);
  printf ("%s : F = %02x\n", FUNC, cpu->AF.r_8.l);

  return cpu;
}

void
cpu_gb_free (CpuGB *cpu) {
  free (cpu);
}

