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

/**
 * Opcode : cpu_ops_8b_and
 * cpu_ops_8b_and (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_and () {
  printf ("test/cpu_ops_8b_and result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_and (&target, 0x00, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && half && !carry);
  cpu_ops_8b_and (&target, 0xFF, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && half && !carry);
  target = 0xFF;
  cpu_ops_8b_and (&target, 0x00, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && half && !carry);
  target = 0xFF;
  cpu_ops_8b_and (&target, 0xFF, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && half && !carry);
  target = 0xFF;
  cpu_ops_8b_and (&target, 0x0F, &zero, &negative, &half, &carry);
  assert (target == 0x0F && !zero && !negative && half && !carry);
  target = 0xFF;
  cpu_ops_8b_and (&target, 0xF0, &zero, &negative, &half, &carry);
  assert (target == 0xF0 && !zero && !negative && half && !carry);
  cpu_ops_8b_and (&target, 0xC0, &zero, &negative, &half, &carry);
  assert (target == 0xC0 && !zero && !negative && half && !carry);
  target = 0xFF;
  cpu_ops_8b_and (&target, 0xAA, &zero, &negative, &half, &carry);
  assert (target == 0xAA && !zero && !negative && half && !carry);
  cpu_ops_8b_and (&target, 0x0F, &zero, &negative, &half, &carry);
  assert (target == 0x0A && !zero && !negative && half && !carry);
  cpu_ops_8b_and (&target, 0xF0, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_or
 * cpu_ops_8b_or (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_or () {
  printf ("test/cpu_ops_8b_or result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_or (&target, 0xFF, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  cpu_ops_8b_or (&target, 0xAA, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  target = 0x0F;
  cpu_ops_8b_or (&target, 0xF0, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_xor
 * cpu_ops_8b_xor (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_xor () {
  printf ("test/cpu_ops_8b_xor result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_xor (&target, 0xFF, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  cpu_ops_8b_xor (&target, 0xFF, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && !half && !carry);
  cpu_ops_8b_xor (&target, 0xAA, &zero, &negative, &half, &carry);
  assert (target == 0xAA && !zero && !negative && !half && !carry);
  cpu_ops_8b_xor (&target, 0x55, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_bit
 * cpu_ops_8b_bit (REG8 *dest, UINT8 bit, UINT32 *zero, UINT32 *negative, UINT32 *half_carry)
 **/
void test_cpu_ops_8b_bit () {
  printf ("test/cpu_ops_8b_bit result: ");
  target = 0xAA;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_bit (&target, 0x00, &zero, &negative, &half);
  assert (target == 0xAA && zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x01, &zero, &negative, &half);
  assert (target == 0xAA && !zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x02, &zero, &negative, &half);
  assert (target == 0xAA && zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x03, &zero, &negative, &half);
  assert (target == 0xAA && !zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x04, &zero, &negative, &half);
  assert (target == 0xAA && zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x05, &zero, &negative, &half);
  assert (target == 0xAA && !zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x06, &zero, &negative, &half);
  assert (target == 0xAA && zero && !negative && half);
  cpu_ops_8b_bit (&target, 0x07, &zero, &negative, &half);
  assert (target == 0xAA && !zero && !negative && half);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_rol
 * cpu_ops_8b_rol (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_rol () {
  printf ("test/cpu_ops_8b_rol result: ");
  target = 0x01;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x02 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x04 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x08 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x10 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x20 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x40 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x80 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x01 && !zero && !negative && !half && carry);
  target = 0x00;
  cpu_ops_8b_rol (&target, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_rlc
 * cpu_ops_8b_rlc (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_rlc () {
  printf ("test/cpu_ops_8b_rlc result: ");
  target = 0x01;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x02 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x04 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x08 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x10 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x20 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x40 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x80 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && !half && carry);
  cpu_ops_8b_rlc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x01 && !zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_ror
 * cpu_ops_8b_ror (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_ror () {
  printf ("test/cpu_ops_8b_ror result: ");
  target = 0x01;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x80 && !zero && !negative && !half && carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x40 && !zero && !negative && !half && !carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x20 && !zero && !negative && !half && !carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x10 && !zero && !negative && !half && !carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x08 && !zero && !negative && !half && !carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x04 && !zero && !negative && !half && !carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x02 && !zero && !negative && !half && !carry);
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x01 && !zero && !negative && !half && !carry);
  target = 0x00;
  cpu_ops_8b_ror (&target, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_rrc
 * cpu_ops_8b_rrc (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_rrc () {
  printf ("test/cpu_ops_8b_rrc result: ");
  target = 0x01;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && !half && carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x80 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x40 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x20 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x10 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x08 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x04 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x02 && !zero && !negative && !half && !carry);
  cpu_ops_8b_rrc (&target, &zero, &negative, &half, &carry);
  assert (target == 0x01 && !zero && !negative && !half && !carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_cmp
 * cpu_ops_8b_cmp (REG8 reg1, REG8 reg2, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_cmp () {
  printf ("test/cpu_ops_8b_cmp result: ");
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_cmp (0x00, 0x00, &zero, &negative, &half, &carry);
  assert (zero && negative && !half && !carry);
  cpu_ops_8b_cmp (0x00, 0xFF, &zero, &negative, &half, &carry);
  assert (!zero && negative && half && carry);
  cpu_ops_8b_cmp (0xFF, 0x00, &zero, &negative, &half, &carry);
  assert (!zero && negative && !half && !carry);
  cpu_ops_8b_cmp (0xFF, 0xFF, &zero, &negative, &half, &carry);
  assert (zero && negative && !half && !carry);
  cpu_ops_8b_cmp (0xFF, 0x0F, &zero, &negative, &half, &carry);
  assert (!zero && negative && !half && !carry);
  cpu_ops_8b_cmp (0xFF, 0xF0, &zero, &negative, &half, &carry);
  assert (!zero && negative && !half && !carry);
  cpu_ops_8b_cmp (0x10, 0x0F, &zero, &negative, &half, &carry);
  assert (!zero && negative && half && !carry);
  cpu_ops_8b_cmp (0x0F, 0xFF, &zero, &negative, &half, &carry);
  assert (!zero && negative && !half && carry);
  cpu_ops_8b_cmp (0x10, 0x8F, &zero, &negative, &half, &carry);
  assert (!zero && negative && half && carry);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_cpl
 * cpu_ops_8b_cpl (REG8 *target, UINT32 *negative, UINT32 *half_carry)
 **/
void test_cpu_ops_8b_cpl () {
  printf ("test/cpu_ops_8b_cpl result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_cpl (&target, &negative, &half);
  assert (target == 0xFF && negative && half);
  cpu_ops_8b_cpl (&target, &negative, &half);
  assert (target == 0x00 && negative && half);
  target = 0xAA;
  cpu_ops_8b_cpl (&target, &negative, &half);
  assert (target == 0x55 && negative && half);
  cpu_ops_8b_cpl (&target, &negative, &half);
  assert (target == 0xAA && negative && half);
  printf ("success\n");
}

/**
 * Opcode : cpu_ops_8b_swap
 * cpu_ops_8b_swap (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry)
 **/
void test_cpu_ops_8b_swap () {
  printf ("test/cpu_ops_8b_swap result: ");
  target = 0x00;
  zero = negative = half = carry = FALSE;
  cpu_ops_8b_swap (&target, &zero, &negative, &half, &carry);
  assert (target == 0x00 && zero && !negative && !half && !carry);
  target = 0xFF;
  cpu_ops_8b_swap (&target, &zero, &negative, &half, &carry);
  assert (target == 0xFF && !zero && !negative && !half && !carry);
  target = 0xF0;
  cpu_ops_8b_swap (&target, &zero, &negative, &half, &carry);
  assert (target == 0x0F && !zero && !negative && !half && !carry);
  cpu_ops_8b_swap (&target, &zero, &negative, &half, &carry);
  assert (target == 0xF0 && !zero && !negative && !half && !carry);
  target = 0xA5;
  cpu_ops_8b_swap (&target, &zero, &negative, &half, &carry);
  assert (target == 0x5A && !zero && !negative && !half && !carry);
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
  test_cpu_ops_8b_and ();
  test_cpu_ops_8b_or ();
  test_cpu_ops_8b_xor ();
  test_cpu_ops_8b_bit ();
  test_cpu_ops_8b_rol ();
  test_cpu_ops_8b_rlc ();
  test_cpu_ops_8b_ror ();
  test_cpu_ops_8b_rrc ();
  test_cpu_ops_8b_cmp ();
  test_cpu_ops_8b_cpl ();
  test_cpu_ops_8b_swap ();
  test_color ();

  return 0;
}