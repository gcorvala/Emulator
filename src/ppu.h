#ifndef _PPU_H_
#define _PPU_H_

#include "types.h"

typedef struct {
  BYTE vram[0x4000]; /* 16 KB */
} PPU; /* 2C02 */

#endif