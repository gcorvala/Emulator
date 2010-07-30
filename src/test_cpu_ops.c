#include <stdio.h>

#include "types.h"
#include "cpu_op_16b.h"

int main () {
  REG8 target = 0x81;
  unsigned int zero = 0, negative = 0, half = 0, carry = 0;

  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_rrc (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);

  printf ("\ntarget : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_op_8b_ror (&target, 1, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  
  return 0;
}