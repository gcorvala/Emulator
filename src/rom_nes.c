#include "rom_nes.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

RomNES *
rom_nes_new (const char *filename) {
  RomNES *rom;
  BYTE header[16];
  int rom_fd;

  rom_fd = open (filename, O_RDONLY);

  if (rom_fd == -1) {
    perror ("rom_nes_new");
    return NULL;
  }

  rom = calloc (sizeof (RomNES), 1);

  if (read (rom_fd, &header, 16) == 0) {
    perror ("rom read");
    close (rom_fd);
    free (rom);
    return NULL;
  }

  if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A) {
    printf ("ERROR - INCORRECT ROM FORMAT!\n");
    close (rom_fd);
    free (rom);
    return NULL;
  }

  rom->prg_page_count = header[4];
  rom->chr_page_count = header[5];

  rom->prg = malloc (sizeof (BYTE) * 16 * 1024 * rom->prg_page_count);
  rom->chr = malloc (sizeof (BYTE) * 8 * 1024 * rom->chr_page_count);

  if (read (rom_fd, rom->prg, sizeof (BYTE) * 16 * 1024 * rom->prg_page_count) != sizeof (BYTE) * 16 * 1024 * rom->prg_page_count) {
    perror ("prg read");
    close (rom_fd);
    free (rom->prg);
    free (rom->chr);
    free (rom);
    return NULL;
  }

  if (read (rom_fd, rom->chr, sizeof (BYTE) * 8 * 1024 * rom->chr_page_count) != sizeof (BYTE) * 8 * 1024 * rom->chr_page_count) {
    perror ("chr read");
    close (rom_fd);
    free (rom->prg);
    free (rom->chr);
    free (rom);
    return NULL;
  }

  close (rom_fd);

  return rom;
}

void
rom_nes_free (RomNES *rom) {
  
}
