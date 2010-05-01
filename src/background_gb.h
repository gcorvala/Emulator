#ifndef _BACKGROUND_GB_H_
#define _BACKGROUND_GB_H_

#include "types.h"

#include <stddef.h>

BackgroundGB * background_gb_new                  (MapGB        *map);
void           background_gb_free                 (BackgroundGB *background);
BOOL           background_gb_is_lcd_enable        (BackgroundGB *background);
BOOL           background_gb_is_window_enable     (BackgroundGB *background);
BOOL           background_gb_is_sprite_enable     (BackgroundGB *background);
BOOL           background_gb_is_background_enable (BackgroundGB *background);
TileGB *       background_gb_get_tile             (BackgroundGB *background, UINT8 n);
size_t         background_gb_get_height           (BackgroundGB *background);
size_t         background_gb_get_width            (BackgroundGB *background);
size_t         background_gb_get_window_height    (BackgroundGB *background);
size_t         background_gb_get_window_width     (BackgroundGB *background);
UINT8          background_gb_get_pixel            (BackgroundGB *background, UINT8 x, UINT8 y);
UINT8          background_gb_get_window_pixel     (BackgroundGB *background, UINT8 x, UINT8 y);

#endif
