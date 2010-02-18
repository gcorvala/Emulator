#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu_6502.h"
#include "rom_nes.h"

const char *ROM_FILENAME = "super.nes";

int
main () {
  RomNES *rom;
  CPU6502 *cpu;

  printf ("Emulator START\n");

  printf ("Load RomNES START\n");

  rom = rom_nes_new (ROM_FILENAME);

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

  cpu = cpu_6502_new ();
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

  printf ("Load CPU6502 END\n");

  free (rom);

  printf ("Emulator END\n");

  return 0;
}
