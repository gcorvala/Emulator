#ifndef _TILE_GB_H_
#define _TILE_GB_H_

#include "types.h"

TileGB * tile_gb_new        (const BYTE *bytes);
void     tile_gb_free       (TileGB *tile);
int      tile_gb_get_pixel  (TileGB *tile, UINT8 x, UINT8 y);

#endif
