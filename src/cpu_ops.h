#ifndef _CPU_OPS_H_
#define _CPU_OPS_H_

#include "types.h"
#define CPU_OPS_MACRO

void  cpu_ops_8b_ld (REG8 *dest, BYTE data);

#ifndef CPU_OPS_MACRO
void  cpu_ops_8b_add (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_adc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_sub (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_sbc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_inc (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero);
void  cpu_ops_8b_dec (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero);
#else
#define cpu_ops_8b_add(dest, value, zero, negative, half_carry, carry) { \
  *zero = (((*dest + value) & 0xFF) == 0x00) ? TRUE : FALSE; \
  *negative = FALSE; \
  *half_carry = (((*dest & 0x0F) + (value & 0x0F)) > 0x0F) ? TRUE : FALSE; \
  *carry = (((*dest) + value) > 0xFF) ? TRUE : FALSE; \
  *dest = (*dest + value); \
}
#define cpu_ops_8b_adc(dest, value, zero, negative, half_carry, carry) { \
  UINT32 n_value = *carry + value; \
  cpu_ops_8b_add(dest, n_value, zero, negative, half_carry, carry); \
}
#define cpu_ops_8b_sub(dest, value, zero, negative, half_carry, carry) { \
  *zero = (*dest == value) ? TRUE : FALSE; \
  *negative = TRUE; \
  *half_carry = ((*dest & 0x0F) < (value & 0x0F)) ? TRUE : FALSE; \
  *carry = (*dest < value) ? TRUE : FALSE; \
  *dest = (*dest - value); \
}
#define cpu_ops_8b_sbc(dest, value, zero, negative, half_carry, carry) { \
  UINT32 n_value = *carry + value; \
  cpu_ops_8b_sub (dest, n_value, zero, negative, half_carry, carry); \
}
#define cpu_ops_8b_inc(dest, negative, half_carry,zero) { \
  *negative = FALSE; \
  *half_carry = ((*dest & 0x0F) == 0x0F) ? TRUE : FALSE; \
  *zero = (*dest == 0xFF) ? TRUE : FALSE; \
  (*dest)++; \
}
#define cpu_ops_8b_dec(dest, negative, half_carry, zero) { \
  *negative = TRUE; \
  *half_carry = ((*dest & 0x0F) == 0x00) ? TRUE : FALSE; \
  *zero = (*dest == 0x01) ? TRUE : FALSE; \
  (*dest)--; \
}
#endif

void  cpu_ops_8b_and (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_or (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_xor (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_bit (REG8 *dest, UINT16 bit, UINT32 *zero, UINT32 *negative, UINT32 *half_carry);
void  cpu_ops_8b_rol (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_rlc (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_ror (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_rrc (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_cmp (REG8 reg1, REG8 reg2, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_cpl (REG8 *target, UINT32 *negative, UINT32 *half_carry);
void  cpu_ops_8b_swap (REG8 *target, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);

void  cpu_ops_16b_ld (REG16 *dest, BYTE first, BYTE second);
void  cpu_ops_16b_inc (REG16 *dest);
void  cpu_ops_16b_dec (REG16 *dest);
void  cpu_ops_16b_add (REG16 *dest, WORD value, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_16b_jp (REG16 *pc, ADDR16 target);
void  cpu_ops_16b_jr (REG16 *pc, INT8 value);
//void  cpu_ops_16b_push (REG16 *pc, UINT16 value);

#endif
