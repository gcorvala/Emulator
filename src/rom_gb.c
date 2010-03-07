#include "rom_gb.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct _RomGB {
  struct {
    BYTE entry[4];
    BYTE logo[48];
    BYTE title[16];
    BYTE new_licence[2];
    BYTE super_flag;
    BYTE type;
    BYTE rom_size;
    BYTE ram_size;
    BYTE destination;
    BYTE old_licence;
    BYTE version;
    BYTE header_checksum;
    BYTE global_checksum[2];
  } header;
};

const BYTE rom_gb_logo[0x30] = {
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

RomGB *
rom_gb_new (const char *filename) {
  RomGB *rom;
  int rom_fd;

  rom_fd = open (filename, O_RDONLY);

  if (rom_fd == -1) {
    perror ("rom_gb_new");
    return NULL;
  }

  rom = calloc (sizeof (RomGB), 1);

  if (lseek (rom_fd, 0x0100, SEEK_SET) == -1) {
    perror ("seek");
    close (rom_fd);
    rom_gb_free (rom);
    return NULL;
  }

  if (read (rom_fd, &(rom->header), 0x50) == 0) {
    perror ("rom read");
    close (rom_fd);
    rom_gb_free (rom);
    return NULL;
  }

  return rom;
}

void
rom_gb_free (RomGB *rom) {
  free (rom);
}

BOOL
rom_gb_check_logo (RomGB *rom) {
  if (memcmp (rom_gb_logo, rom->header.logo, 0x30))
    return FALSE;
  else
    return TRUE;
}

char *
rom_gb_get_title (RomGB *rom) {
  char *title;

  title = calloc (sizeof (char), 16);
  memcpy (title, rom->header.title, 16);

  return title;
}

size_t
rom_gb_get_rom_size (RomGB *rom) {
  size_t size;

  size = 32 << (rom->header.rom_size & 0x0f);
  if (rom->header.rom_size & 0xf0)
    size += 32 << (rom->header.rom_size & 0xf0);
  size *= 1024;

  return size;
}

size_t
rom_gb_get_ram_size (RomGB *rom) {
  size_t size;

  if (rom->header.ram_size == 0)
    size = 0;
  else if (rom->header.ram_size == 1)
    size = 2 * 1024;
  else if (rom->header.ram_size == 2)
    size = 8 * 1024;
  else if (rom->header.ram_size == 3)
    size = 32 * 1024;

  return size;
}

BYTE *
rom_gb_get_licence (RomGB *rom) {
  if (rom->header.old_licence == 0x33)
    return rom->header.new_licence;
  else
    return &(rom->header.old_licence);
}
