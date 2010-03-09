#include "cpu_gb.h"

struct _CpuGB {
  REG8 reg_a;
  REG8 reg_x;
  REG8 reg_y;
  REG8 reg_status;
  REG8 reg_sp; /* $0100-$01FF */
  REG16 reg_ip;
  BYTE ram[0x0800]; /* 2KB */
};
