#ifndef _CPU_GB_H_
#define _CPU_GB_H_

#include "types.h"

typedef struct _CpuGB CpuGB;

CpuGB * cpu_gb_new  (void);
void    cpu_gb_free (CpuGB *cpu);
void    cpu_gb_set_zero_flag (CpuGB *cpu);
void    cpu_gb_set_substract_flag (CpuGB *cpu);
void    cpu_gb_set_half_carry_flag (CpuGB *cpu);
void    cpu_gb_set_carry_flag (CpuGB *cpu);

void    cpu_gb_step (CpuGB *cpu);

#endif
