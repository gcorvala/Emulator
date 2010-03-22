#include "display_gb.h"

#include "map_gb.h"
#include "tile_gb.h"

#include <stdlib.h>
#include <stdio.h>

struct _DisplayGB {
  MapGB *map;
};

DisplayGB *
display_gb_new (MapGB *map) {
  DisplayGB *display;

  display = calloc (sizeof (DisplayGB), 1);

  display->map = map;

  return display;
}

void
display_gb_free (DisplayGB *display) {
  free (display);
}

TileGB *
display_gb_get_tile (DisplayGB *display, UINT8 n) {
  TileGB *result;

  if (n >= 0xC0) {
    printf ("%s !!! tile number is invalid (max 192) !!!\n", FUNC);
    return NULL;
  }

  result = tile_gb_new (map_gb_get_memory (display->map, 0x8000 + n * 16 + 0),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 1),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 2),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 3),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 4),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 5),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 6),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 7),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 8),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 9),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 10),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 11),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 12),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 13),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 14),
                        map_gb_get_memory (display->map, 0x8000 + n * 16 + 15));

  return result;
}

void
display_gb_print_test (DisplayGB *display) {
  TileGB *tile;
  int logo[16][96];
  int i, j, k;

  for (i = 0; i < 16; ++i)
    for (j = 0; j < 64; ++j)
      logo[i][j] = 9;

  for (i = 0; i < 12; ++i) {
    tile = display_gb_get_tile (display, i + 1);
    for (j = 0; j < 8; ++j) {
      for (k = 0; k < 8; ++k) {
        logo[k][i * 8 + j] = tile_gb_get_pixel (tile, k, j);
      }
    }
    tile_gb_free (tile);
    tile = display_gb_get_tile (display, i + 13);
    for (j = 0; j < 8; ++j) {
      for (k = 0; k < 8; ++k) {
        logo[k + 8][i * 8 + j] = tile_gb_get_pixel (tile, k, j);
      }
    }
    tile_gb_free (tile);
  }

  for (i = 0; i < 16; ++i) {
    for (j = 0; j < 96; ++j) {
      /*printf ("%s", logo[i][j] ? "X" : " ");*/
      printf ("%d", logo[i][j]);
    }
    printf ("\n");
  }
}