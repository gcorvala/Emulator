#include "background_gb.h"

#include "map_gb.h"
#include "tile_gb.h"

#include <stdlib.h>
#include <stdio.h>

struct _BackgroundGB {
  MapGB *map;
  size_t height;
  size_t width;
  size_t w_height;
  size_t w_width;
};

BackgroundGB *
background_gb_new (MapGB *map) {
  BackgroundGB *background;

  background = calloc (sizeof (BackgroundGB), 1);

  background->map = map;
  background->height = 256;
  background->width = 256;
  background->w_height = 144;
  background->w_width = 160;

  return background;
}

void
background_gb_free (BackgroundGB *background) {
  free (background);
}

BOOL
background_gb_is_lcd_enable (BackgroundGB *background) {
  return map_gb_get_memory (background->map, 0xFF40) & 0x80 ? TRUE : FALSE;
}

BOOL
background_gb_is_window_enable (BackgroundGB *background) {
  return map_gb_get_memory (background->map, 0xFF40) & 0x20 ? TRUE : FALSE;
}

BOOL
background_gb_is_sprite_enable (BackgroundGB *background) {
  return map_gb_get_memory (background->map, 0xFF40) & 0x02 ? TRUE : FALSE;
}

BOOL
background_gb_is_background_enable (BackgroundGB *background) {
  return map_gb_get_memory (background->map, 0xFF40) & 0x01 ? TRUE : FALSE;
}


TileGB *
background_gb_get_tile (BackgroundGB *background, INT8 n) {
  TileGB *result;
  BYTE bytes[16];
  ADDR16 tile_data;
  int i;

  tile_data = map_gb_get_memory (background->map, 0xFF40) & 0x10 ? 0x8000 : 0x9000;
  for (i = 0; i < 16; ++i) {
    bytes[i] = map_gb_get_memory (background->map, tile_data + n * 16 + i);
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

size_t
background_gb_get_window_height (BackgroundGB *background) {
  return background->w_height;
}

size_t
background_gb_get_window_width (BackgroundGB *background) {
  return background->w_width;
}

UINT8
background_gb_get_pixel (BackgroundGB *background, UINT8 x, UINT8 y) {
  int tile_id;
  TileGB *tile;
  UINT8 map_tile_x, map_tile_y;
  UINT8 tile_x, tile_y;
  ADDR16 bg_map;
  int result;

  if (background_gb_is_background_enable (background) == FALSE ||
      background_gb_is_lcd_enable (background) == FALSE)
    return 0;
  bg_map = map_gb_get_memory (background->map, 0xFF40) & 0x08 ? 0x9C00 : 0x9800;
  map_tile_x = x >> 3; /* div 8 */
  map_tile_y = y >> 3; /* div 8 */
  tile_id = map_gb_get_memory (background->map, bg_map + map_tile_x + map_tile_y * 32);
  tile = background_gb_get_tile (background, tile_id);
  tile_x = x & 7; /* mod 8 */
  tile_y = y & 7; /* mod 8 */
  result = tile_gb_get_pixel (tile, tile_x, tile_y);
  tile_gb_free (tile);

  return result;
}

UINT8
background_gb_get_window_pixel (BackgroundGB *background, UINT8 x, UINT8 y) {
  UINT8 scroll_x, scroll_y;

  scroll_x = map_gb_get_memory (background->map, 0xFF43);
  scroll_y = map_gb_get_memory (background->map, 0xFF42);

  return background_gb_get_pixel (background, x + scroll_x, y + scroll_y);
}