#include "rom_gb.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
  BYTE entry[4];
  BYTE logo[48];
  BYTE title[15];
  BYTE color_flag;
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
} RomGBHeader;

struct _RomGB {
  RomGBHeader *header;
  BYTE **banks;
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
  BYTE *bank0;
  unsigned int nb_banks, i;

  rom_fd = open (filename, O_RDONLY);

  if (rom_fd == -1) {
    perror ("rom_gb_new");
    return NULL;
  }

  rom = calloc (sizeof (RomGB), 1);

  bank0 = calloc (sizeof (BYTE), 0x4000);
  if (read (rom_fd, bank0, 0x4000) == 0) {
    perror ("rom read");
    close (rom_fd);
    rom_gb_free (rom);
    return NULL;
  }

  rom->header = (RomGBHeader *) (bank0 + 0x0100);
  
  nb_banks = rom_gb_get_rom_size (rom) / (1024 * 16);
  rom->banks = calloc (sizeof (BYTE *), nb_banks);
  rom->banks[0] = bank0;
  for (i = 1; i < nb_banks; ++i) {
    rom->banks[i] = calloc (sizeof (BYTE), 0x4000);
    if (read (rom_fd, rom->banks[i], 0x4000) == 0) {
      perror ("rom read");
    };
  }

  close (rom_fd);

  return rom;
}

void
rom_gb_free (RomGB *rom) {
  unsigned int nb_banks, i;

  nb_banks = rom_gb_get_rom_size (rom) / (1024 * 16);
  for (i = 0; i < nb_banks; ++i) {
    free (rom->banks[i]);
  }
  free (rom->banks);
  free (rom);
}

char *
rom_gb_get_title (RomGB *rom) {
  char *title;
  size_t size;

  if (rom_gb_has_color_flag (rom) == TRUE)
    size = 15;
  else
    size = 16;

  title = malloc (sizeof (char) * (size + 1));
  memcpy (title, rom->header->title, size);
  title[size] = 0;

  return title;
}

UINT16
rom_gb_get_licence (RomGB *rom) {
  UINT16 licence = 0x0000;
  char new_licence[2];

  if (rom->header->old_licence == 0x33) {
    new_licence[0] = rom->header->new_licence[0];
    new_licence[1] = rom->header->new_licence[1];
    licence = atoi (new_licence);
  }
  else
    licence = rom->header->old_licence;

  return licence;
}

UINT8
rom_gb_get_type (RomGB *rom) {
  return rom->header->type;
}

size_t
rom_gb_get_rom_size (RomGB *rom) {
  size_t size;

  size = 32 << (rom->header->rom_size & 0x0f);
  if (rom->header->rom_size & 0xf0)
    size += 32 << (rom->header->rom_size & 0xf0);
  size *= 1024;

  return size;
}

size_t
rom_gb_get_ram_size (RomGB *rom) {
  size_t size;

  if (rom->header->ram_size == 0)
    size = 0;
  else if (rom->header->ram_size == 1)
    size = 2 * 1024;
  else if (rom->header->ram_size == 2)
    size = 8 * 1024;
  else if (rom->header->ram_size == 3)
    size = 32 * 1024;
  else
    size = 0;

  return size;
}

UINT8
rom_gb_get_version (RomGB *rom) {
  return rom->header->version;
}

BOOL
rom_gb_has_color_flag (RomGB *rom) {
  if (rom->header->color_flag == 0x80 || rom->header->color_flag == 0xC0)
    return TRUE;
  else
    return FALSE;
}

BOOL
rom_gb_has_super_flag (RomGB *rom) {
  if (rom->header->super_flag == 0x03)
    return TRUE;
  else
    return FALSE;
}

BOOL
rom_gb_is_for_japan (RomGB *rom) {
  if (rom->header->destination == 0x00)
    return TRUE;
  else
    return FALSE;
}

BOOL
rom_gb_check_logo (RomGB *rom) {
  if (memcmp (rom_gb_logo, rom->header->logo, 0x30))
    return FALSE;
  else
    return TRUE;
}

BOOL
rom_gb_check_header (RomGB *rom) {
  int i, x;

  x = 0;
  for (i = 0x0134; i <= 0x014C; ++i) {
    x = x - rom->banks[0][i] - 1;
  }

  if (rom->header->header_checksum == (x & 0xFF))
    return TRUE;
  else
    return FALSE;
}

BOOL
rom_gb_check_full (RomGB *rom) {
  unsigned int nb_banks, i, j, checksum = 0;

  nb_banks = rom_gb_get_rom_size (rom) / (1024 * 16);

  for (i = 0; i < nb_banks; ++i) {
    for (j = 0; j < 0x4000; ++j) {
      if (i != 0 || (j != 0x014E && j != 0x014F)) {
        checksum += rom->banks[i][j];
        checksum &= 0xffff;
      }
    }
  }

  if (((checksum & 0xff00) >> 8) == rom->header->global_checksum[0] &&
      (checksum & 0x00ff) == rom->header->global_checksum[1])
    return TRUE;
  else
    return FALSE;
}
