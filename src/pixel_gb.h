#ifndef _PIXEL_GB_H_
#define _PIXEL_GB_H_

#include "types.h"

typedef enum {
  PIXEL_GB_COLOR_1 = 0,
  PIXEL_GB_COLOR_2 = 1,
  PIXEL_GB_COLOR_3 = 2,
  PIXEL_GB_COLOR_4 = 3,
} PixelGBColor;

PixelGB *    pixel_gb_new       (PixelGBColor color);
void         pixel_gb_free      (PixelGB *pixel);
PixelGBColor pixel_gb_get_color (PixelGB *pixel);

#endif
