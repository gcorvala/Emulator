#include "cpu.h"

#include "types.h"

#include <stdlib.h>
#include <stdio.h>

void
cpu_free (Cpu *cpu) {
  if (cpu->free == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return;
  }

  cpu->free (cpu);
}

UINT8
cpu_step (Cpu *cpu) {
  if (cpu->step == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return 0;
  }

  return cpu->step (cpu);
}