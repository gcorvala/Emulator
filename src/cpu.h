#ifndef _CPU_H_
#define _CPU_H_

#include "types.h"

typedef void (*CpuSetFlag) (Cpu *cpu, BOOL value);
typedef BOOL (*CpuGetFlag) (Cpu *cpu);

struct _Cpu {
  void (*free) (Cpu *cpu);
  UINT8 (*step) (Cpu *cpu);
  CpuGetFlag get_zero_flag;
  CpuSetFlag set_zero_flag;
  CpuGetFlag get_negative_flag;
  CpuSetFlag set_negative_flag;
  CpuGetFlag get_carry_flag;
  CpuSetFlag set_carry_flag;
  CpuGetFlag get_half_carry_flag;
  CpuSetFlag set_half_carry_flag;
};

void    cpu_free                (Cpu *cpu);
UINT8   cpu_step                (Cpu *cpu);
BOOL    cpu_get_zero_flag       (Cpu *cpu);
void    cpu_set_zero_flag       (Cpu *cpu, BOOL value);
BOOL    cpu_get_negative_flag   (Cpu *cpu);
void    cpu_set_negative_flag   (Cpu *cpu, BOOL value);
BOOL    cpu_get_carry_flag      (Cpu *cpu);
void    cpu_set_carry_flag      (Cpu *cpu, BOOL value);
BOOL    cpu_get_half_carry_flag (Cpu *cpu);
void    cpu_set_half_carry_flag (Cpu *cpu, BOOL value);

#endif
