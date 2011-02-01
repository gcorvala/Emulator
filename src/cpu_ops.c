#include <stdio.h>
#include "cpu_ops.h"

void
cpu_ops_8b_ld (REG8 *dest, BYTE data) {
  *dest = data;
}

void
cpu_ops_8b_add (REG8 *dest, REG8 value, CpuGBFlags *flags) {
  flags->zero_flag = (((*dest + value) & 0xFF) == 0x00) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = (((*dest & 0x0F) + (value & 0x0F)) > 0x0F) ? TRUE : FALSE;
  flags->carry_flag = ((*dest + value) > 0xFF) ? TRUE : FALSE;
  *dest += value;
}

void
cpu_ops_8b_adc (REG8 *dest, REG8 value, CpuGBFlags *flags) {
  cpu_ops_8b_add (dest, value + flags->carry_flag, flags);
}

void
cpu_ops_8b_sub (REG8 *dest, REG8 value, CpuGBFlags *flags) {
  flags->zero_flag = (*dest == value) ? TRUE : FALSE;
  flags->negative_flag = TRUE;
  flags->half_carry_flag = ((*dest & 0x0F) < (value & 0x0F)) ? TRUE : FALSE;
  flags->carry_flag = (*dest < value) ? TRUE : FALSE;
  *dest -= value;
}

void
cpu_ops_8b_sbc (REG8 *dest, REG8 value, CpuGBFlags *flags) {
  cpu_ops_8b_sub (dest, (value + flags->carry_flag), flags);
}

void
cpu_ops_8b_inc (REG8 *dest, CpuGBFlags *flags) {
  flags->negative_flag = FALSE;
  flags->half_carry_flag = ((*dest & 0x0F) == 0x0F) ? TRUE : FALSE;
  flags->zero_flag = (*dest == 0xFF) ? TRUE : FALSE;
  (*dest)++;
}

void
cpu_ops_8b_dec (REG8 *dest, CpuGBFlags *flags) {
  flags->negative_flag = TRUE;
  flags->half_carry_flag = ((*dest & 0x0F) == 0x00) ? TRUE : FALSE;
  flags->zero_flag = (*dest == 0x01) ? TRUE : FALSE;
  (*dest)--;
}

void
cpu_ops_8b_and (REG8 *dest, BYTE value, CpuGBFlags *flags) {
  *dest &= value;
  flags->zero_flag = (*dest == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = TRUE;
  flags->carry_flag = FALSE;
}

void
cpu_ops_8b_or (REG8 *dest, BYTE value, CpuGBFlags *flags) {
  *dest |= value;
  flags->zero_flag = (*dest == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
  flags->carry_flag = FALSE;
}

void
cpu_ops_8b_xor (REG8 *dest, BYTE value, CpuGBFlags *flags) {
  *dest ^= value;
  flags->zero_flag = (*dest == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
  flags->carry_flag = FALSE;
}

void
cpu_ops_8b_bit (REG8 *dest, UINT16 bit, CpuGBFlags *flags) {
  UINT8 mask = 0x01 << bit;
  flags->zero_flag = (*dest & mask) == 0 ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = TRUE;
}

void
cpu_ops_8b_rol (REG8 *target, CpuGBFlags *flags) {
  flags->carry_flag = *target >> 7;
  *target = (*target << 1) | flags->carry_flag;
  flags->zero_flag = (*target == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
}

void
cpu_ops_8b_rlc (REG8 *target, CpuGBFlags *flags) {
  UINT32 c = flags->carry_flag;
  flags->carry_flag = *target >> 7;
  *target = (*target << 1) | c;
  flags->zero_flag = (*target == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
}

void
cpu_ops_8b_ror (REG8 *target, CpuGBFlags *flags) {
  flags->carry_flag = *target & 0x01;
  *target = (*target >> 1) | (flags->carry_flag << 7);
  flags->zero_flag = (*target == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
}

void
cpu_ops_8b_rrc (REG8 *target, CpuGBFlags *flags) {
  BOOL c = flags->carry_flag;
  flags->carry_flag = *target & 0x01;
  *target = (*target >> 1) | (c << 7);
  flags->zero_flag = (*target == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
} 

void
cpu_ops_8b_cmp (REG8 reg1, REG8 reg2, CpuGBFlags *flags) {
  flags->zero_flag = (reg1 == reg2) ? TRUE : FALSE;
  flags->negative_flag = TRUE;
  flags->half_carry_flag = ((reg1 & 0x0F) < (reg2 & 0x0F)) ? TRUE : FALSE;
  flags->carry_flag = (reg1 < reg2) ? TRUE : FALSE;
}

void
cpu_ops_8b_cpl (REG8 *target, CpuGBFlags *flags) {
  *target = ~(*target);
  flags->negative_flag = TRUE;
  flags->half_carry_flag = TRUE; 
}

void
cpu_ops_8b_swap (REG8 *target, CpuGBFlags *flags) {
  *target = (*target << 4) | (*target >> 4);
  flags->zero_flag = (*target == 0) ? TRUE : FALSE;
  flags->negative_flag = FALSE;
  flags->half_carry_flag = FALSE;
  flags->carry_flag = FALSE;
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
cpu_ops_16b_add (REG16 *dest, WORD value, CpuGBFlags *flags) {
  flags->negative_flag = FALSE;
  flags->half_carry_flag = (dest->r_8.l + (value & 0xFF) > 0xFF) ? TRUE : FALSE;
  flags->carry_flag = (dest->r_16 + value > 0xFFFF) ? TRUE : FALSE;
  dest->r_16 +=  value;
}

void
cpu_ops_16b_jp (REG16 *pc, ADDR16 target) {
  pc->r_16 = target;
}

void
cpu_ops_16b_jr (REG16 *pc, INT8 value) {
  //printf ("old : %04x | value : %d | new : %04x\n", pc->r_16, value, pc->r_16 + value);
  if (pc->r_16 != 0x0342)
    pc->r_16 += value;
}