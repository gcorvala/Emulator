#include "display_gb.h"

#include <stdlib.h>

struct {
  BYTE tile_data[0x1800];
  BYTE background[];
} _DisplayGB;

DisplayGB *
display_gb_new (void) {
  return NULL;
}

void
display_gb_free (DisplayGB *display) {
  free (display);
}
