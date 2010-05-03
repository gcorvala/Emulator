#ifndef _COLOR_H_
#define _COLOR_H_

#include "types.h"

Color * color_new       (UINT8 red, UINT8 green, UINT8 blue);
void    color_free      (Color *color);
UINT8   color_get_red   (Color *color);
UINT8   color_get_green (Color *color);
UINT8   color_get_blue  (Color *color);
void    color_set_red   (Color *color, UINT8 red);
void    color_set_green (Color *color, UINT8 green);
void    color_set_blue  (Color *color, UINT8 blue);

#endif
