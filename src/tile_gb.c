#include "tile_gb.h"

#include <stdlib.h>
#include <stdio.h>

struct _TileGB {
  int pixels[8][8];
};

TileGB *
tile_gb_new (const BYTE bytes[16]) {
  TileGB *tile;
  int i, j;

  tile = calloc (sizeof (TileGB), 1);

  for (i = 0; i < 8; ++i) {
    for (j = 0; j < 8; ++j) {
      tile->pixels[i][j] = ((bytes[j * 2] >> (7 - i)) & 0x01) | ((bytes[j * 2 + 1] >> (7 - i - 1)) & 0x02);
    }
  }

  return tile;
}

void
tile_gb_free (TileGB *tile) {
  free (tile);
}

int
tile_gb_get_pixel (TileGB *tile, UINT8 x, UINT8 y) {
  if (x > 7 || y > 7)
    printf ("%s !!! tile is 8x8 !!! [x : %u | y : %u]\n", FUNC, x, y);
  return tile->pixels[x][y];
}