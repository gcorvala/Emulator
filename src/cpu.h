#ifndef _CPU_H_
#define _CPU_H_

#include "types.h"

struct _Cpu {
  void (*free) (Cpu *cpu);
  UINT8 (*step) (Cpu *cpu);
};

void    cpu_free                (Cpu *cpu);
UINT8   cpu_step                (Cpu *cpu);

#endif
