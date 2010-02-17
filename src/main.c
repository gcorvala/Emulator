#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu_6502.h"
#include "rom_nes.h"

const char *ROM_FILENAME = "super.nes";

int
main () {
  int rom_fd;
  RomNES *rom;
  CPU6502 *cpu;
  BYTE h[16];

  printf ("Emulator START\n");

  printf ("Load RomNES START\n");

  rom_fd = open (ROM_FILENAME, O_RDONLY);

  if (rom_fd == -1) {
    perror ("open");
    return 1;
  }

  rom = malloc (sizeof (RomNES));

  if (read (rom_fd, &h, 16) == 0) {
    perror ("rom read");
    free (rom);
    return 1;
  }

  if (h[0] != 'N' || h[1] != 'E' || h [2] != 'S' || h[3] != 0x1A) {
    printf ("ERROR - INCORRECT rom!\n");
    free (rom);
    return 1;
  }

  rom->prg_page_count = h[4];
  rom->chr_page_count = h[5];

  rom->prg = malloc (sizeof (BYTE) * 16 * 1024 * rom->prg_page_count);
  rom->chr = malloc (sizeof (BYTE) * 8 * 1024 * rom->chr_page_count);

  if (read (rom_fd, rom->prg, sizeof (BYTE) * 16 * 1024 * rom->prg_page_count) != sizeof (BYTE) * 16 * 1024 * rom->prg_page_count) {
    perror ("prg read");
    free (rom->prg);
    free (rom->chr);
    free (rom);
    return 1;
  }

  if (read (rom_fd, rom->chr, sizeof (BYTE) * 8 * 1024 * rom->chr_page_count) != sizeof (BYTE) * 8 * 1024 * rom->chr_page_count) {
    perror ("chr read");
    free (rom->prg);
    free (rom->chr);
    free (rom);
    return 1;
  }

  printf ("\trom->prg_page_count = %d\n", rom->prg_page_count);
  printf ("\trom->chr_page_count %d\n", rom->chr_page_count);
  printf ("\trom->prg start\n");
  printf ("\t\t%x%x%x%x %x%x%x%x\n", rom->prg[0] & 0xff
                               , rom->prg[1] & 0xff
                               , rom->prg[2] & 0xff
                               , rom->prg[3] & 0xff
                               , rom->prg[4] & 0xff
                               , rom->prg[5] & 0xff
                               , rom->prg[6] & 0xff
                               , rom->prg[7] & 0xff);
  printf ("\trom->prg end\n");
  printf ("\t\t%x%x%x%x %x%x%x%x\n", rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 8] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 7] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 6] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 5] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 4] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 3] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 2] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 1] & 0xff);
  printf ("\trom->chr start\n");
  printf ("\t\t%x%x%x%x %x%x%x%x\n", rom->chr[0] & 0xff
                               , rom->chr[1] & 0xff
                               , rom->chr[2] & 0xff
                               , rom->chr[3] & 0xff
                               , rom->chr[4] & 0xff
                               , rom->chr[5] & 0xff
                               , rom->chr[6] & 0xff
                               , rom->chr[7] & 0xff);
  printf ("\trom->chr end\n");
  printf ("\t\t%x%x%x%x %x%x%x%x\n", rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 8] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 7] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 6] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 5] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 4] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 3] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 2] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 1] & 0xff);


  printf ("Load RomNES END\n");

  printf ("Load CPU6502 START\n");

  cpu = malloc (sizeof (CPU6502));
  cpu_6502_load_rom (cpu, rom);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);
  cpu_6502_step (cpu);

  printf ("Load CPU6502 END\n");

  free (rom);

  printf ("Emulator END\n");

  return 0;
}