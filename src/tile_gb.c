#include "tile_gb.h"

#include <stdlib.h>
#include <stdio.h>

struct _TileGB {
  int pixels[8][8];
};

TileGB *
tile_gb_new (BYTE one, BYTE two) {
  TileGB *tile;

  /*printf ("%02x - %02x\n", one, two);*/

  tile = calloc (sizeof (TileGB), 1);

  tile->pixels[0][0] = ((one & 0x80) >> 7) | ((two & 0x80) >> 6);
  tile->pixels[0][1] = ((one & 0x40) >> 6) | ((two & 0x40) >> 5);
  tile->pixels[0][2] = ((one & 0x20) >> 5) | ((two & 0x20) >> 4);
  tile->pixels[0][3] = ((one & 0x10) >> 4) | ((two & 0x10) >> 3);
  tile->pixels[0][4] = ((one & 0x08) >> 3) | ((two & 0x08) >> 2);
  tile->pixels[0][5] = ((one & 0x04) >> 2) | ((two & 0x04) >> 1);
  tile->pixels[0][6] = ((one & 0x02) >> 1) | ((two & 0x02) >> 0);
  tile->pixels[0][7] = ((one & 0x01) >> 0) | ((two & 0x01) << 1);

  printf ("\t> %d %d %d %d %d %d %d %d <\n", tile->pixels[0][0],
                                           tile->pixels[0][1],
                                           tile->pixels[0][2],
                                           tile->pixels[0][3],
                                           tile->pixels[0][4],
                                           tile->pixels[0][5],
                                           tile->pixels[0][6],
                                           tile->pixels[0][7]);
  return tile;
}

void
tile_gb_free (TileGB *tile) {
  free (tile);
}