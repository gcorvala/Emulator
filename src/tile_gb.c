#include "tile_gb.h"

#include <stdlib.h>
#include <stdio.h>

struct _TileGB {
  int pixels[8][8];
};

TileGB *
tile_gb_new (BYTE l11, BYTE l12,
             BYTE l21, BYTE l22,
             BYTE l31, BYTE l32,
             BYTE l41, BYTE l42,
             BYTE l51, BYTE l52,
             BYTE l61, BYTE l62,
             BYTE l71, BYTE l72,
             BYTE l81, BYTE l82) {
  TileGB *tile;

  tile = calloc (sizeof (TileGB), 1);

  tile->pixels[0][0] = ((l11 & 0x80) >> 7) | ((l12 & 0x80) >> 6);
  tile->pixels[0][1] = ((l11 & 0x40) >> 6) | ((l12 & 0x40) >> 5);
  tile->pixels[0][2] = ((l11 & 0x20) >> 5) | ((l12 & 0x20) >> 4);
  tile->pixels[0][3] = ((l11 & 0x10) >> 4) | ((l12 & 0x10) >> 3);
  tile->pixels[0][4] = ((l11 & 0x08) >> 3) | ((l12 & 0x08) >> 2);
  tile->pixels[0][5] = ((l11 & 0x04) >> 2) | ((l12 & 0x04) >> 1);
  tile->pixels[0][6] = ((l11 & 0x02) >> 1) | ((l12 & 0x02) >> 0);
  tile->pixels[0][7] = ((l11 & 0x01) >> 0) | ((l12 & 0x01) << 1);

  tile->pixels[1][0] = ((l21 & 0x80) >> 7) | ((l22 & 0x80) >> 6);
  tile->pixels[1][1] = ((l21 & 0x40) >> 6) | ((l22 & 0x40) >> 5);
  tile->pixels[1][2] = ((l21 & 0x20) >> 5) | ((l22 & 0x20) >> 4);
  tile->pixels[1][3] = ((l21 & 0x10) >> 4) | ((l22 & 0x10) >> 3);
  tile->pixels[1][4] = ((l21 & 0x08) >> 3) | ((l22 & 0x08) >> 2);
  tile->pixels[1][5] = ((l21 & 0x04) >> 2) | ((l22 & 0x04) >> 1);
  tile->pixels[1][6] = ((l21 & 0x02) >> 1) | ((l22 & 0x02) >> 0);
  tile->pixels[1][7] = ((l21 & 0x01) >> 0) | ((l22 & 0x01) << 1);

  tile->pixels[2][0] = ((l31 & 0x80) >> 7) | ((l32 & 0x80) >> 6);
  tile->pixels[2][1] = ((l31 & 0x40) >> 6) | ((l32 & 0x40) >> 5);
  tile->pixels[2][2] = ((l31 & 0x20) >> 5) | ((l32 & 0x20) >> 4);
  tile->pixels[2][3] = ((l31 & 0x10) >> 4) | ((l32 & 0x10) >> 3);
  tile->pixels[2][4] = ((l31 & 0x08) >> 3) | ((l32 & 0x08) >> 2);
  tile->pixels[2][5] = ((l31 & 0x04) >> 2) | ((l32 & 0x04) >> 1);
  tile->pixels[2][6] = ((l31 & 0x02) >> 1) | ((l32 & 0x02) >> 0);
  tile->pixels[2][7] = ((l31 & 0x01) >> 0) | ((l32 & 0x01) << 1);

  tile->pixels[3][0] = ((l41 & 0x80) >> 7) | ((l42 & 0x80) >> 6);
  tile->pixels[3][1] = ((l41 & 0x40) >> 6) | ((l42 & 0x40) >> 5);
  tile->pixels[3][2] = ((l41 & 0x20) >> 5) | ((l42 & 0x20) >> 4);
  tile->pixels[3][3] = ((l41 & 0x10) >> 4) | ((l42 & 0x10) >> 3);
  tile->pixels[3][4] = ((l41 & 0x08) >> 3) | ((l42 & 0x08) >> 2);
  tile->pixels[3][5] = ((l41 & 0x04) >> 2) | ((l42 & 0x04) >> 1);
  tile->pixels[3][6] = ((l41 & 0x02) >> 1) | ((l42 & 0x02) >> 0);
  tile->pixels[3][7] = ((l41 & 0x01) >> 0) | ((l42 & 0x01) << 1);

  tile->pixels[4][0] = ((l51 & 0x80) >> 7) | ((l52 & 0x80) >> 6);
  tile->pixels[4][1] = ((l51 & 0x40) >> 6) | ((l52 & 0x40) >> 5);
  tile->pixels[4][2] = ((l51 & 0x20) >> 5) | ((l52 & 0x20) >> 4);
  tile->pixels[4][3] = ((l51 & 0x10) >> 4) | ((l52 & 0x10) >> 3);
  tile->pixels[4][4] = ((l51 & 0x08) >> 3) | ((l52 & 0x08) >> 2);
  tile->pixels[4][5] = ((l51 & 0x04) >> 2) | ((l52 & 0x04) >> 1);
  tile->pixels[4][6] = ((l51 & 0x02) >> 1) | ((l52 & 0x02) >> 0);
  tile->pixels[4][7] = ((l51 & 0x01) >> 0) | ((l52 & 0x01) << 1);

  tile->pixels[5][0] = ((l61 & 0x80) >> 7) | ((l62 & 0x80) >> 6);
  tile->pixels[5][1] = ((l61 & 0x40) >> 6) | ((l62 & 0x40) >> 5);
  tile->pixels[5][2] = ((l61 & 0x20) >> 5) | ((l62 & 0x20) >> 4);
  tile->pixels[5][3] = ((l61 & 0x10) >> 4) | ((l62 & 0x10) >> 3);
  tile->pixels[5][4] = ((l61 & 0x08) >> 3) | ((l62 & 0x08) >> 2);
  tile->pixels[5][5] = ((l61 & 0x04) >> 2) | ((l62 & 0x04) >> 1);
  tile->pixels[5][6] = ((l61 & 0x02) >> 1) | ((l62 & 0x02) >> 0);
  tile->pixels[5][7] = ((l61 & 0x01) >> 0) | ((l62 & 0x01) << 1);

  tile->pixels[6][0] = ((l71 & 0x80) >> 7) | ((l72 & 0x80) >> 6);
  tile->pixels[6][1] = ((l71 & 0x40) >> 6) | ((l72 & 0x40) >> 5);
  tile->pixels[6][2] = ((l71 & 0x20) >> 5) | ((l72 & 0x20) >> 4);
  tile->pixels[6][3] = ((l71 & 0x10) >> 4) | ((l72 & 0x10) >> 3);
  tile->pixels[6][4] = ((l71 & 0x08) >> 3) | ((l72 & 0x08) >> 2);
  tile->pixels[6][5] = ((l71 & 0x04) >> 2) | ((l72 & 0x04) >> 1);
  tile->pixels[6][6] = ((l71 & 0x02) >> 1) | ((l72 & 0x02) >> 0);
  tile->pixels[6][7] = ((l71 & 0x01) >> 0) | ((l72 & 0x01) << 1);

  tile->pixels[7][0] = ((l81 & 0x80) >> 7) | ((l82 & 0x80) >> 6);
  tile->pixels[7][1] = ((l81 & 0x40) >> 6) | ((l82 & 0x40) >> 5);
  tile->pixels[7][2] = ((l81 & 0x20) >> 5) | ((l82 & 0x20) >> 4);
  tile->pixels[7][3] = ((l81 & 0x10) >> 4) | ((l82 & 0x10) >> 3);
  tile->pixels[7][4] = ((l81 & 0x08) >> 3) | ((l82 & 0x08) >> 2);
  tile->pixels[7][5] = ((l81 & 0x04) >> 2) | ((l82 & 0x04) >> 1);
  tile->pixels[7][6] = ((l81 & 0x02) >> 1) | ((l82 & 0x02) >> 0);
  tile->pixels[7][7] = ((l81 & 0x01) >> 0) | ((l82 & 0x01) << 1);

  return tile;
}

void
tile_gb_free (TileGB *tile) {
  free (tile);
}

int
tile_gb_get_pixel (TileGB *tile, UINT8 x, UINT8 y) {
  if (x > 7 || y > 7)
    printf ("%s !!! tile is 8x8 !!!\n", FUNC);
  return tile->pixels[x][y];
}

void
tile_gb_printf (TileGB *tile) {
  int i, j;

  for (i = 0; i < 8; ++i) {
    printf ("> ");
    for (j = 0; j < 8; ++j) {
      printf ("%d", tile->pixels[i][j] ? 1 : 0);
    }
    printf (" <\n");
  }
}
