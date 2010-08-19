#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "color.h"
#include "cpu_ops.h"

REG8 target;
UINT32 zero, negative, half, carry;

void test_type_sizes () {
  printf ("test/type_sizes result: ");
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
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_add
 * void cpu_ops_8b_add (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_add () {
  printf ("test/cpu_ops_8b_add result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_add (&target, 0x0F, &zero, &negative, &half, &carry);
  assert (target == 0x0F && !zero && !negative && !half && !carry);
  cpu_ops_8b_add (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0x10 && !zero && !negative && half && !carry);
  cpu_ops_8b_add (&target, 0xEF, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  cpu_ops_8b_add (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && half && carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_adc
 * cpu_ops_8b_adc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_adc () {
  printf ("test/cpu_ops_8b_adc result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_adc (&target, 0xFF, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  cpu_ops_8b_adc (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && half && carry);
  cpu_ops_8b_adc (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0x02 && !zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_sub
 * cpu_ops_8b_sub (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_sub () {
  printf ("test/cpu_ops_8b_sub result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_sub (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && negative && half && carry);
  cpu_ops_8b_sub (&target, 0x0F, &zero, &negative, &half, &carry);
  assert (target == 0xF0 && !zero && negative && !half && !carry);
  cpu_ops_8b_sub (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0xEF && !zero && negative && half && !carry);
  cpu_ops_8b_sub (&target, 0xE0, &zero, &negative, &half, &carry);
  assert (target == 0x0F && !zero && negative && !half && !carry);
  cpu_ops_8b_sub (&target, 0x0F, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_sbc
 * cpu_ops_8b_sbc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_sbc () {
  printf ("test/cpu_ops_8b_sbc result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_sbc (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && negative && half && carry);
  cpu_ops_8b_sbc (&target, 0x01, &zero, &negative, &half, &carry);
  assert (target == 0xFD && !zero && negative && !half && !carry);
  cpu_ops_8b_sbc (&target, 0x0FE, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && negative && half && carry);
  cpu_ops_8b_sbc (&target, 0x0FE, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_inc
 * cpu_ops_8b_inc (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero)
 **/
void test_cpu_ops_8b_inc () {
  printf ("test/cpu_ops_8b_inc result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0x01 && !negative && !half && !zero);
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0x02 && !negative && !half && !zero);
  target = 0x0E;
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0x0F && !negative && !half && !zero);
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0x10 && !negative && half && !zero);
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0x11 && !negative && !half && !zero);
  target = 0xFE;
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0xFF && !negative && !half && !zero);
  cpu_ops_8b_inc (&target, &negative, &half, &zero);
  assert (target == 0x00 && !negative && half && zero);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_dec
 * cpu_ops_8b_dec (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero)
 **/
void test_cpu_ops_8b_dec () {
  printf ("test/cpu_ops_8b_dec result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_dec (&target, &negative, &half, &zero);
  assert (target == 0xFF && negative && half && !zero);
  cpu_ops_8b_dec (&target, &negative, &half, &zero);
  assert (target == 0xFE && negative && !half && !zero);
  target = 0x02;
  cpu_ops_8b_dec (&target, &negative, &half, &zero);
  assert (target == 0x01 && negative && !half && !zero);
  cpu_ops_8b_dec (&target, &negative, &half, &zero);
  assert (target == 0x00 && negative && !half && zero);
  printf ("success\n");
}

void test_color () {
  printf ("test/color result: ");

  Color *color = color_new (0x11, 0x22, 0x33);

  assert (color_get_red (color) == 0x11);
  assert (color_get_green (color) == 0x22);
  assert (color_get_blue (color) == 0x33);

  color_free (color);

  printf ("success\n");
}

int main () {

  test_type_sizes ();
  test_cpu_ops_8b_add ();
  test_cpu_ops_8b_adc ();
  test_cpu_ops_8b_sub ();
  test_cpu_ops_8b_sbc ();
  test_cpu_ops_8b_inc ();
  test_cpu_ops_8b_dec ();
  test_color ();

  return 0;
}