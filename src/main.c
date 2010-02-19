#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu.h"
#include "rom.h"
#include "types.h"

const char *ROM_FILENAME = "super.nes";

int
main () {
  Rom *rom;
  CPU *cpu;
  Mapper *mapper;
  ADDR addr;

  printf ("Emulator START\n");

  printf ("Load RomNES START\n");

  rom = rom_new (ROM_FILENAME);

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

  for (addr = 0x8000; addr != 0x8008; ++addr) {
    printf ("\tcpu->mem->[0x%04x] = %02x\n", addr, cpu_get_memory (cpu, addr) & 0xff);
  }

  for (addr = 0xFFF8; addr != 0x0000; ++addr) {
    printf ("\tcpu->mem->[0x%04x] = %02x\n", addr, cpu_get_memory (cpu, addr) & 0xff);
  }

  printf ("Load CPU END\n");

  rom_free (rom);
  cpu_free (cpu);

  printf ("Emulator END\n");

  return 0;
}
