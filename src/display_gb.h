#ifndef _DISPLAY_GB_H_
#define _DISPLAY_GB_H_

#include "types.h"

DisplayGB * display_gb_new      (MapGB     *map);
void        display_gb_free     (DisplayGB *display);
TileGB    * display_gb_get_tile (DisplayGB *display, UINT8 n);

void        display_gb_print_test (DisplayGB *display);

#endif
