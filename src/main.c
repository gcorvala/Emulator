#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu.h"
#include "rom_nes.h"

const char *ROM_FILENAME = "super.nes";

int
main () {
  RomNES *rom;
  CPU *cpu;
  Mapper *mapper;

  printf ("Emulator START\n");

  printf ("Load RomNES START\n");

  rom = rom_nes_new (ROM_FILENAME);

  printf ("\trom->prg_page_count = %d\n", rom->prg_page_count);
  printf ("\trom->chr_page_count %d\n", rom->chr_page_count);
  printf ("\trom->prg start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom->prg[0] & 0xff
                               , rom->prg[1] & 0xff
                               , rom->prg[2] & 0xff
                               , rom->prg[3] & 0xff
                               , rom->prg[4] & 0xff
                               , rom->prg[5] & 0xff
                               , rom->prg[6] & 0xff
                               , rom->prg[7] & 0xff);
  printf ("\trom->prg end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 8] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 7] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 6] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 5] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 4] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 3] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 2] & 0xff
                               , rom->prg[sizeof (BYTE) * 16 * 1024 * rom->prg_page_count - 1] & 0xff);
  printf ("\trom->chr start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom->chr[0] & 0xff
                               , rom->chr[1] & 0xff
                               , rom->chr[2] & 0xff
                               , rom->chr[3] & 0xff
                               , rom->chr[4] & 0xff
                               , rom->chr[5] & 0xff
                               , rom->chr[6] & 0xff
                               , rom->chr[7] & 0xff);
  printf ("\trom->chr end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 8] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 7] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 6] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 5] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 4] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 3] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 2] & 0xff
                               , rom->chr[sizeof (BYTE) * 8 * 1024 * rom->chr_page_count - 1] & 0xff);


  printf ("Load RomNES END\n");

  printf ("Load CPU START\n");

  mapper = mapper_new (rom);

  cpu = cpu_new ();
  cpu_set_mapper (cpu, mapper);
  cpu_load_rom (cpu, rom);
  /*cpu_step (cpu);
  cpu_step (cpu);
  cpu_step (cpu);
  cpu_step (cpu);
  cpu_step (cpu);
  cpu_step (cpu);
  cpu_step (cpu);
  cpu_step (cpu);*/

  printf ("cpu->mem->[0x0000] = %02x\n", cpu_get_memory (cpu, 0x0000));
  cpu_set_memory (cpu, 0x0000, 0x48);
  printf ("cpu->mem->[0x0000] = %02x\n", cpu_get_memory (cpu, 0x0000));
  printf ("cpu->mem->[0x1000] = %02x\n", cpu_get_memory (cpu, 0x1000));
  cpu_set_memory (cpu, 0x1600, 0x96);
  printf ("cpu->mem->[0x1600] = %02x\n", cpu_get_memory (cpu, 0x1600));
  printf ("cpu->mem->[0x0600] = %02x\n", cpu_get_memory (cpu, 0x0600));

  printf ("cpu->mem->[0x8000] = %02x\n", cpu_get_memory (cpu, 0x8000));
  printf ("cpu->mem->[0x8001] = %02x\n", cpu_get_memory (cpu, 0x8001));
  printf ("cpu->mem->[0x8002] = %02x\n", cpu_get_memory (cpu, 0x8002));
  printf ("cpu->mem->[0x8003] = %02x\n", cpu_get_memory (cpu, 0x8003));
  printf ("cpu->mem->[0x8004] = %02x\n", cpu_get_memory (cpu, 0x8004));
  printf ("cpu->mem->[0x8005] = %02x\n", cpu_get_memory (cpu, 0x8005));
  printf ("cpu->mem->[0x8006] = %02x\n", cpu_get_memory (cpu, 0x8006));
  printf ("cpu->mem->[0x8007] = %02x\n", cpu_get_memory (cpu, 0x8007));

//  printf ("cpu->mem->[0xFFF6] = %02x\n", cpu_get_memory (cpu, 0xFFF6));
//  printf ("cpu->mem->[0xFFF7] = %02x\n", cpu_get_memory (cpu, 0xFFF7));
  printf ("cpu->mem->[0xFFF8] = %02x\n", cpu_get_memory (cpu, 0xFFF8));
  printf ("cpu->mem->[0xFFF9] = %02x\n", cpu_get_memory (cpu, 0xFFF9));
  printf ("cpu->mem->[0xFFFA] = %02x\n", cpu_get_memory (cpu, 0xFFFA));
  printf ("cpu->mem->[0xFFFB] = %02x\n", cpu_get_memory (cpu, 0xFFFB));
  printf ("cpu->mem->[0xFFFC] = %02x\n", cpu_get_memory (cpu, 0xFFFC));
  printf ("cpu->mem->[0xFFFD] = %02x\n", cpu_get_memory (cpu, 0xFFFD));
  printf ("cpu->mem->[0xFFFE] = %02x\n", cpu_get_memory (cpu, 0xFFFE));
  printf ("cpu->mem->[0xFFFF] = %02x\n", cpu_get_memory (cpu, 0xFFFF));

  printf ("Load CPU END\n");

  free (rom);
  free (cpu);

  printf ("Emulator END\n");

  return 0;
}
