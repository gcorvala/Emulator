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

BOOL
cpu_get_zero_flag (Cpu *cpu) {
  if (cpu->get_zero_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return FALSE;
  }

  return cpu->get_zero_flag (cpu);
}

void
cpu_set_zero_flag (Cpu *cpu, BOOL value) {
  if (cpu->set_zero_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return;
  }

  cpu->set_zero_flag (cpu, value);
}

BOOL
cpu_get_negative_flag (Cpu *cpu) {
  if (cpu->get_negative_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return FALSE;
  }

  return cpu->get_negative_flag (cpu);
}

void
cpu_set_negative_flag (Cpu *cpu, BOOL value) {
  if (cpu->set_negative_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return;
  }

  cpu->set_negative_flag (cpu, value);
}

BOOL
cpu_get_carry_flag (Cpu *cpu) {
  if (cpu->get_carry_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return FALSE;
  }

  return cpu->get_carry_flag (cpu);
}

void
cpu_set_carry_flag (Cpu *cpu, BOOL value) {
  if (cpu->set_carry_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return;
  }

  cpu->set_carry_flag (cpu, value);
}

BOOL
cpu_get_half_carry_flag (Cpu *cpu) {
  if (cpu->get_half_carry_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return FALSE;
  }

  return cpu->get_half_carry_flag (cpu);
}

void
cpu_set_half_carry_flag (Cpu *cpu, BOOL value) {
  if (cpu->set_half_carry_flag == NULL) {
    printf ("%s : function not defined !\n", FUNC);
    return;
  }

  cpu->set_half_carry_flag (cpu, value);
}