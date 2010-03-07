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

  printf ("%s : %x %x %x %x\n", FUNC,
                                rom->header.entry[0],
                                rom->header.entry[1],
                                rom->header.entry[2],
                                rom->header.entry[3]);
  printf ("%s : rom size = %u\n", FUNC, rom->header.rom_size);
  printf ("%s : ram size = %u\n", FUNC, rom->header.ram_size);
  printf ("%s : type = %u\n", FUNC, rom->header.type);

  return rom;
}

void
rom_gb_free (RomGB *rom) {
  free (rom);
}

char *
rom_gb_get_title (RomGB *rom) {
  char *title;

  title = calloc (sizeof (char), 16);
  memcpy (title, rom->header.title, 16);

  return title;
}
