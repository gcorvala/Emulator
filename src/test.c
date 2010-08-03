#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "color.h"
#include "cpu_ops.h"

int main () {
  REG8 target = 0x81;
  UINT32 zero = 0, negative = 0, half = 0, carry = 0;

  printf ("Test types START\n");

  assert (sizeof (INT8) == 1);
  assert (sizeof (INT16) == 2);
  assert (sizeof (INT32) == 4);
  assert (sizeof (UINT8) == 1);
  assert (sizeof (UINT16) == 2);
  assert (sizeof (UINT32) == 4);
  assert (sizeof (ADDR8) == 1);
  assert (sizeof (ADDR16) == 2);
  assert (sizeof (ADDR32) == 4);
  assert (sizeof (REG8) == 1);
  assert (sizeof (REG16) == 2);
  assert (sizeof (REG32) == 4);
  assert (sizeof (BYTE) == 1);
  assert (sizeof (WORD) == 2);
  assert (sizeof (DWORD) == 4);
  assert (TRUE != 0);
  assert (FALSE == 0);

  printf ("Test types END\n");

  printf ("Test Color START\n");

  Color *color = color_new (0x11, 0x22, 0x33);

  assert (color_get_red (color) == 0x11);
  assert (color_get_green (color) == 0x22);
  assert (color_get_blue (color) == 0x33);

  color_free (color);

  printf ("Test Color END\n");

  printf ("target : %2x | carry : %2x\n", target, carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  printf ("target : %2x | carry : %2x\n", target, carry);
  
  return 0;
}