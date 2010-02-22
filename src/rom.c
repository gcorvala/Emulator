#include "rom.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct _Rom {
  unsigned int prg_n_pages;
  unsigned int chr_n_pages;
  BYTE *prg;
  BYTE *chr;
  int mirroring_flag : 1;
  int sram_flag : 1;
  int trainer_flag : 1;
  int four_screen_flag : 1;
  unsigned short int mapper_id;
};

Rom *
rom_new (const char *filename) {
  Rom *rom;
  BYTE header[16];
  int rom_fd;

  rom_fd = open (filename, O_RDONLY);

  if (rom_fd == -1) {
    perror ("rom_nes_new");
    return NULL;
  }

  rom = calloc (sizeof (Rom), 1);

  if (read (rom_fd, &header, 16) == 0) {
    perror ("rom read");
    close (rom_fd);
    rom_free (rom);
    return NULL;
  }

  if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A) {
    printf ("ERROR - INCORRECT ROM FORMAT!\n");
    close (rom_fd);
    rom_free (rom);
    return NULL;
  }

  rom->prg_n_pages = header[4];
  rom->chr_n_pages = header[5];

  rom->mirroring_flag = header[6] & 0x01;
  rom->sram_flag = (header[6] & 0x02) >> 1;
  rom->trainer_flag = (header[6] & 0x04) >> 2;
  rom->four_screen_flag = (header[6] & 0x08) >> 3;

  rom->mapper_id = (header[6] >> 4) | (header[7] & 0xf0);

  rom->prg = malloc (rom_get_prg_size (rom));

  if (read (rom_fd, rom->prg, rom_get_prg_size (rom)) != rom_get_prg_size (rom)) {
    perror ("prg read");
    close (rom_fd);
    rom_free (rom);
    return NULL;
  }

  rom->chr = malloc (rom_get_chr_size (rom));

  if (read (rom_fd, rom->chr, rom_get_chr_size (rom)) != rom_get_chr_size (rom)) {
    perror ("chr read");
    close (rom_fd);
    rom_free (rom);
    return NULL;
  }

  close (rom_fd);

  return rom;
}

void
rom_free (Rom *rom) {
  if (rom->prg) {
    free (rom->prg);
  }
  if (rom->chr) {
    free (rom->chr);
  }
  free (rom);
}

BYTE
rom_get_prg_memory (Rom *rom, ADDR16 addr) {
  return *(rom->prg + addr);
}

unsigned int
rom_get_prg_n_pages (Rom *rom) {
  return rom->prg_n_pages;
}

size_t
rom_get_prg_size (Rom *rom) {
  return 16 * 1024 * rom->prg_n_pages;
}

BYTE
rom_get_chr_memory (Rom *rom, ADDR16 addr) {
  return *(rom->chr + addr);
}

unsigned int
rom_get_chr_n_pages (Rom *rom) {
  return rom->chr_n_pages;
}

size_t
rom_get_chr_size (Rom *rom) {
  return 8 * 1024 * rom->chr_n_pages;
}

int
rom_get_mirroring_flag (Rom *rom) {
  return rom->mirroring_flag;
}

int
rom_get_sram_flag (Rom *rom) {
  return rom->sram_flag;
}

int
rom_get_trainer_flag (Rom *rom) {
  return rom->trainer_flag;
}

int
rom_get_four_screen_flag (Rom *rom) {
  return rom->four_screen_flag;
}

unsigned short int
rom_get_mapper_id (Rom *rom) {
  return rom->mapper_id;
}