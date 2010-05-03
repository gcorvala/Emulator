#include "color.h"

#include <stdlib.h>

struct _Color {
  UINT8 red;
  UINT8 green;
  UINT8 blue;
};

Color *
color_new (UINT8 red, UINT8 green, UINT8 blue) {
  Color *color;

  color = malloc (sizeof (Color));

  color->red = red;
  color->green = green;
  color->blue = blue;

  return color;
}

void
color_free (Color *color) {
  free (color);
}

UINT8
color_get_red (Color *color) {
  return color->red;
}

UINT8
color_get_green (Color *color) {
  return color->green;
}

UINT8
color_get_blue (Color *color) {
  return color->blue;
}

void
color_set_red (Color *color, UINT8 red) {
  color->red = red;
}

void
color_set_green (Color *color, UINT8 green) {
  color->green = green;
}

void
color_set_blue (Color *color, UINT8 blue) {
  color->blue = blue;
}