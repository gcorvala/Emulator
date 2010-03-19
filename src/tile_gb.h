#ifndef _TILE_GB_H_
#define _TILE_GB_H_

#include "types.h"

#include <stddef.h>

TileGB * tile_gb_new        (BYTE one, BYTE two);
void     tile_gb_free       (TileGB *tile);
size_t * tile_gb_get_height (TileGB *tile);
size_t * tile_gb_get_width  (TileGB *tile);
void     tile_gb_get_size   (TileGB *tile, size_t *height, size_t *width);

#endif
