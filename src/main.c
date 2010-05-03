#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu_gb.h"
#include "rom_gb.h"
#include "map_gb.h"
#include "tile_gb.h"
#include "background_gb.h"

#include "color.h"
#include "types.h"

#include "SDL.h"

int
main (int argc, char **argv) {
  RomGB *rom_gb;
  CpuGB *cpu_gb;
  MapGB *map_gb;
  TileGB *tile_gb;
  BackgroundGB *background_gb;
  char *tmp;
  INT32 i, j, k;
  BYTE array[16] = { 0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFF, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00};
  UINT32 cycles = 0;
  Color *color;

  printf ("argc : %d\n", argc);
  for (i = 0; i < argc; ++i) {
    printf ("argv[%d] : %s\n", i, argv[i]);
  }

  printf ("Test types START\n");

  printf ("\tsizeof (INT8) = %lu bits\n", (unsigned long) sizeof (INT8) * 8);
  printf ("\tsizeof (INT16) = %lu bits\n", (unsigned long) sizeof (INT16) * 8);
  printf ("\tsizeof (INT32) = %lu bits\n", (unsigned long) sizeof (INT32) * 8);
  printf ("\tsizeof (UINT8) = %lu bits\n", (unsigned long) sizeof (UINT8) * 8);
  printf ("\tsizeof (UINT16) = %lu bits\n", (unsigned long) sizeof (UINT16) * 8);
  printf ("\tsizeof (UINT32) = %lu bits\n", (unsigned long) sizeof (UINT32) * 8);
  printf ("\tsizeof (ADDR8) = %lu bits\n", (unsigned long) sizeof (ADDR8) * 8);
  printf ("\tsizeof (ADDR16) = %lu bits\n", (unsigned long) sizeof (ADDR16) * 8);
  printf ("\tsizeof (ADDR32) = %lu bits\n", (unsigned long) sizeof (ADDR32) * 8);
  printf ("\tsizeof (REG8) = %lu bits\n", (unsigned long) sizeof (REG8) * 8);
  printf ("\tsizeof (REG16) = %lu bits\n", (unsigned long) sizeof (REG16) * 8);
  printf ("\tsizeof (REG32) = %lu bits\n", (unsigned long) sizeof (REG32) * 8);

  printf ("\tTRUE = %d\n", TRUE);
  printf ("\tFALSE = %d\n", FALSE);

  printf ("Test types END\n");

  printf ("Test Color START\n");

  color = color_new (0x11, 0x22, 0x33);

  printf ("\tColor->red = %02x\n", color_get_red (color));
  printf ("\tColor->green = %02x\n", color_get_green (color));
  printf ("\tColor->blue = %02x\n", color_get_blue (color));

  color_free (color);

  printf ("Test Color END\n");

  printf ("Emulator START\n");

  printf ("Load RomGB START\n");

  rom_gb = rom_gb_new ("../data/boxxle.gb");
  tmp = rom_gb_get_title (rom_gb);
  printf ("\trom_gb_get_title = %s\n", tmp);
  free (tmp);
  printf ("\trom_gb_get_licence = %u\n", rom_gb_get_licence (rom_gb));
  printf ("\trom_gb_get_type = %02x\n", rom_gb_get_type (rom_gb));
  printf ("\trom_gb_get_rom_size = %lu KB\n", (unsigned long) rom_gb_get_rom_size (rom_gb) / 1024);
  printf ("\trom_gb_get_ram_size = %lu KB\n", (unsigned long) rom_gb_get_ram_size (rom_gb) / 1024);
  printf ("\trom_gb_get_version = %u\n", rom_gb_get_version (rom_gb));
  printf ("\trom_gb_has_color_flag = %s\n", rom_gb_has_color_flag (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_has_super_flag = %s\n", rom_gb_has_super_flag (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_is_for_japan = %s\n", rom_gb_is_for_japan (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_check_logo = %s\n", rom_gb_check_logo (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_check_header = %s\n", rom_gb_check_header (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_check_full = %s\n", rom_gb_check_full (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_get_rom_memory (rom_gb, 0, 0x00A8) : %02x\n", rom_gb_get_rom_memory (rom_gb, 0, 0x00A8));

  printf ("Load RomGB END\n");

  printf ("Test TileGB START\n");

  tile_gb = tile_gb_new (array);
  for (i = 0; i < 8; ++i) {
    for (j = 0; j < 8; ++j) {
      printf ("%u", tile_gb_get_pixel (tile_gb, j, i));
    }
    printf ("\n");
  }
  tile_gb_free (tile_gb);

  printf ("Test TileGB END\n");

  printf ("Load CpuGB START\n");

  cpu_gb = cpu_gb_new ();
  map_gb = map_gb_new ();
  background_gb = background_gb_new (map_gb);
  map_gb_set_cpu (map_gb, cpu_gb);
  map_gb_set_rom (map_gb, rom_gb);
  cpu_gb_set_mapper (cpu_gb, map_gb);

  {
    SDL_Surface *surface;
    SDL_Rect *bg;
    Color *color;
    Uint32 white;
    Uint32 *pixmem;

    if (SDL_Init (SDL_INIT_VIDEO) != 0) {
      return 1;
    }

    surface = SDL_SetVideoMode (background_gb_get_window_width (background_gb), background_gb_get_window_height (background_gb), 32, SDL_HWSURFACE);

    bg = calloc (sizeof (SDL_Rect), 1);
    bg->w = background_gb_get_window_width (background_gb);
    bg->h = background_gb_get_window_height (background_gb);

    white = SDL_MapRGB (surface->format, 0xFF, 0xFF, 0xFF);

    SDL_FillRect (surface, bg, white);

    for (k = 0; k < 0x2FFFF; ++k) {
      cycles += cpu_gb_step (cpu_gb);
      /*printf ("CYCLES %u\n", cycles);*/
 
      if (cycles % 4560 == 0) {
        SDL_FillRect (surface, bg, white);
        for (i = 0; i < (INT32) background_gb_get_window_height (background_gb) + 10; ++i) {
          /*map_gb_set_memory (map_gb, 0xFF44, i);*/
          if (i < 144) {
            for (j = 0; j < (INT32) background_gb_get_window_width (background_gb); ++j) {
              color = background_gb_get_window_pixel (background_gb, j, i);
              if (color != NULL) {
                Uint32 sdl_color;
                sdl_color = SDL_MapRGB (surface->format, color_get_red (color), color_get_green (color), color_get_blue (color));
                pixmem = ((Uint32*) (surface->pixels) + j + i * surface->w);
                *pixmem = sdl_color;
              }
            }
          }
        }
        SDL_UpdateRect (surface, 0, 0, 0, 0);
      }
    }

    free (bg);

    SDL_Delay (1000);
    
    SDL_FreeSurface (surface);
  }

  background_gb_free (background_gb);

  printf ("Load CpuGB END\n");

  printf ("Load MapGB START\n");

  printf ("\tmap_gb_get_memory (0x0134) = %c\n", map_gb_get_memory (map_gb, 0x134));
  map_gb_free (map_gb);
  rom_gb_free (rom_gb);
  cpu_gb_free (cpu_gb);

  printf ("Load MapGB END\n");

  printf ("Emulator END\n");

  return 0;
}
