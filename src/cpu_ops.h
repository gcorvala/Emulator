#ifndef _CPU_OPS_H_
#define _CPU_OPS_H_

#include "types.h"

void  cpu_ops_8b_ld (REG8 *dest, BYTE data);
void  cpu_ops_8b_add (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_adc (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_sub (REG8 *dest, REG8 value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_inc (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero);
void  cpu_ops_8b_dec (REG8 *dest, UINT32 *negative, UINT32 *half_carry, UINT32 *zero);
void  cpu_ops_8b_and (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_or (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_xor (REG8 *dest, BYTE value, UINT32 *zero, UINT32 *negative, UINT32 *half_carry, UINT32 *carry);
void  cpu_ops_8b_bit (REG8 *dest, unsigned short int bit, UINT32 *zero, UINT32 *negative, UINT32 *half_carry);
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
