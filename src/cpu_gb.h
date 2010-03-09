#ifndef _CPU_GB_H_
#define _CPU_GB_H_

#include "types.h"

typedef struct _CpuGB CpuGB;

CpuGB * cpu_gb_new  (void);
void    cpu_gb_free (CpuGB *cpu);

#endif
