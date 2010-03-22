#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu_gb.h"
#include "rom_gb.h"
#include "map_gb.h"
#include "tile_gb.h"
#include "display_gb.h"

#include "types.h"

int
main () {
  RomGB *rom_gb;
  CpuGB *cpu_gb;
  MapGB *map_gb;
  DisplayGB *display_gb;
  char *tmp;
  int i;

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

  printf ("Load CpuGB START\n");

  cpu_gb = cpu_gb_new ();
  map_gb = map_gb_new ();
  map_gb_set_cpu (map_gb, cpu_gb);
  map_gb_set_rom (map_gb, rom_gb);
  cpu_gb_set_mapper (cpu_gb, map_gb);

  for (i = 0; i < 0xBB40; ++i)
    cpu_gb_step (cpu_gb);

  printf ("Load CpuGB END\n");

  printf ("Test TileGB START\n");

  display_gb = display_gb_new (map_gb);
  display_gb_print_test (display_gb);
  display_gb_background_normal (display_gb);
  display_gb_free (display_gb);

  printf ("Test TileGB END\n");

  printf ("Load MapGB START\n");

  printf ("\tmap_gb_get_memory (0x0134) = %c\n", map_gb_get_memory (map_gb, 0x134));
  map_gb_free (map_gb);
  rom_gb_free (rom_gb);
  cpu_gb_free (cpu_gb);

  printf ("Load MapGB END\n");

  printf ("Emulator END\n");

  return 0;
}
