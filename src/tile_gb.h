#ifndef _TILE_GB_H_
#define _TILE_GB_H_

#include "types.h"

TileGB * tile_gb_new        (BYTE l11, BYTE l12,
                             BYTE l21, BYTE l22,
                             BYTE l31, BYTE l32,
                             BYTE l41, BYTE l42,
                             BYTE l51, BYTE l52,
                             BYTE l61, BYTE l62,
                             BYTE l71, BYTE l72,
                             BYTE l81, BYTE l82);
void     tile_gb_free       (TileGB *tile);
int      tile_gb_get_pixel  (TileGB *tile, UINT8 x, UINT8 y);
void     tile_gb_printf     (TileGB *tile);
#endif
