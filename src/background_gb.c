#include "background_gb.h"

#include "map_gb.h"
#include "tile_gb.h"

#include <stdlib.h>
#include <stdio.h>

struct _BackgroundGB {
  MapGB *map;
  size_t height;
  size_t width;
};

BackgroundGB *
background_gb_new (MapGB *map) {
  BackgroundGB *background;

  background = calloc (sizeof (BackgroundGB), 1);

  background->map = map;
  background->height = 256;
  background->width = 256;

  return background;
}

void
background_gb_free (BackgroundGB *background) {
  free (background);
}

TileGB *
background_gb_get_tile (BackgroundGB *background, UINT8 n) {
  TileGB *result;
  BYTE bytes[16];
  int i;

  if (n >= 0xC0) {
    printf ("%s !!! tile number is invalid (max 192) !!!\n", FUNC);
    return NULL;
  }

  for (i = 0; i < 16; ++i) {
    bytes[i] = map_gb_get_memory (background->map, 0x8000 + n * 16 + i);
  }

  result = tile_gb_new (bytes);

  return result;
}

size_t
background_gb_get_height (BackgroundGB *background) {
  return background->height;
}

size_t
background_gb_get_width (BackgroundGB *background) {
  return background->width;
}

UINT8
background_gb_get_pixel (BackgroundGB *background, UINT8 x, UINT8 y) {
  int tile_id;
  TileGB *tile;
  UINT8 map_tile_x, map_tile_y;
  UINT8 tile_x, tile_y;
  int result;

  map_tile_x = x >> 3; /* div 8 */
  map_tile_y = y >> 3; /* div 8 */
  tile_x = x & 7; /* mod 8 */
  tile_y = y & 7; /* mod 8 */
  tile_id = map_gb_get_memory (background->map, 0x9800 + map_tile_x + map_tile_y * 32);
  tile = background_gb_get_tile (background, tile_id);
  result = tile_gb_get_pixel (tile, tile_x, tile_y);
  tile_gb_free (tile);

  return result;
}