#include <stdio.h>
#include "cpu_ops.h"

void
cpu_ops_8b_ld (REG8 *dest, BYTE data) {
  *dest = data;
}

#ifndef CPU_OPS_MACRO
void
cpu_ops_8b_add (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *zero = (((*dest + value) & 0xFF) == 0x00) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = (((*dest & 0x0F) + (value & 0x0F)) > 0x0F) ? TRUE : FALSE;
  *carry = ((*dest + value) > 0xFF) ? TRUE : FALSE;
  *dest += value;
}

void
cpu_ops_8b_adc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  cpu_ops_8b_add (dest, value + *carry, zero, negative, half_carry, carry);
}

void
cpu_ops_8b_sub (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *zero = (*dest == value) ? TRUE : FALSE;
  *negative = TRUE;
  *half_carry = ((*dest & 0x0F) < (value & 0x0F)) ? TRUE : FALSE;
  *carry = (*dest < value) ? TRUE : FALSE;
  *dest -= value;
}

void
cpu_ops_8b_sbc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  cpu_ops_8b_sub (dest, (value + *carry), zero, negative, half_carry, carry);
}

void
cpu_ops_8b_inc (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero) {
  *negative = FALSE;
  *half_carry = ((*dest & 0x0F) == 0x0F) ? TRUE : FALSE;
  *zero = (*dest == 0xFF) ? TRUE : FALSE;
  (*dest)++;
}

void
cpu_ops_8b_dec (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero) {
  *negative = TRUE;
  *half_carry = ((*dest & 0x0F) == 0x00) ? TRUE : FALSE;
  *zero = (*dest == 0x01) ? TRUE : FALSE;
  (*dest)--;
}
#endif

void
cpu_ops_8b_and (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *dest &= value;
  *zero = (*dest == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = TRUE;
  *carry = FALSE;
}

void
cpu_ops_8b_or (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *dest |= value;
  *zero = (*dest == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
  *carry = FALSE;
}

void
cpu_ops_8b_xor (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *dest ^= value;
  *zero = (*dest == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
  *carry = FALSE;
}

void
cpu_ops_8b_bit (REG8 *dest, unsigned short int bit, UINT32 *zero, UINT32 *negative, UINT32 *half_carry) {
  UINT8 mask = 0x01 << bit;
  *zero = (*dest & mask) == 0 ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = TRUE;
}

void
cpu_ops_8b_rol (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  BOOL c = *carry;
  *carry = *target >> 7;
  *target = (*target << 1) | c;
  *zero = (*target == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
}

void
cpu_ops_8b_rlc (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *carry = *target >> 7;
  *target = (*target << 1) | *carry;
  *zero = (*target == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
}

void
cpu_ops_8b_ror (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  BOOL c = *carry;
  *carry = *target & 0x01;
  *target = (*target >> 1) | (c << 7);
  *zero = (*target == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
}

void
cpu_ops_8b_rrc (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *carry = *target & 0x01;
  *target = (*target >> 1) | (*carry << 7);
  *zero = (*target == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
} 

void
cpu_ops_8b_cmp (REG8 reg1, REG8 reg2, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *zero = (reg1 == reg2) ? TRUE : FALSE;
  *negative = TRUE;
  *half_carry = ((reg1 & 0x0F) < (reg2 & 0x0F)) ? TRUE : FALSE;
  *carry = (reg1 < reg2) ? TRUE : FALSE;
}

void
cpu_ops_8b_cpl (REG8 *target, UINT32 *negative, UINT32 *half_carry) {
  *target = ~(*target);
  *negative = TRUE;
  *half_carry = TRUE; 
}

void
cpu_ops_8b_swap (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *target = (*target << 4) | (*target >> 4);
  *zero = (*target == 0) ? TRUE : FALSE;
  *negative = FALSE;
  *half_carry = FALSE;
  *carry = FALSE;
}

void
cpu_ops_16b_ld (REG16 *dest, BYTE first, BYTE second) {
  dest->r_8.l = first;
  dest->r_8.h = second;
}

void
cpu_ops_16b_inc (REG16 *dest) {
  dest->r_16++;
}

void
cpu_ops_16b_dec (REG16 *dest) {
  dest->r_16--;
}

void
cpu_ops_16b_add (REG16 *dest, WORD value, UINT32 *negative, UINT32 *half_carry, UINT32 *carry) {
  *negative = FALSE;
  *half_carry = (dest->r_8.l + (value & 0xFF) > 0xFF) ? TRUE : FALSE;
  *carry = (dest->r_16 + value > 0xFFFF) ? TRUE : FALSE;
  dest->r_16 +=  value;
}

void
cpu_ops_16b_jp (REG16 *pc, ADDR16 target) {
  pc->r_16 = target;
}

void
cpu_ops_16b_jr (REG16 *pc, INT8 value) {
  printf ("old : %04x | value : %d | new : %04x\n", pc->r_16, value, pc->r_16 + value);
  if (pc->r_16 != 0x0342)
    pc->r_16 += value;
}