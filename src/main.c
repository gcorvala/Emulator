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

  printf ("Emulator START\n");

  printf ("Load RomNES START\n");

  rom = rom_new (ROM_FILENAME);

  printf ("\tprg_n_pages = %u\n", rom_get_prg_n_pages (rom));
  printf ("\tprg_size = %zu KB\n", rom_get_prg_size (rom) / 1024);
  printf ("\tchr_n_pages = %u\n", rom_get_chr_n_pages (rom));
  printf ("\tchr_size = %zu KB\n", rom_get_chr_size (rom) / 1024);
  printf ("\tmirroring_flag = %d\n", rom_get_mirroring_flag (rom));
  printf ("\tsram_flag = %d\n", rom_get_sram_flag (rom));
  printf ("\ttrainer_flag = %d\n", rom_get_trainer_flag (rom));
  printf ("\tfour_screen_flag = %d\n", rom_get_four_screen_flag (rom));
  printf ("\tmapper_id = %d\n", rom_get_mapper_id (rom));

  printf ("\trom->prg start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_get_prg_memory (rom, 0) & 0xff
                                                   , rom_get_prg_memory (rom, 1) & 0xff
                                                   , rom_get_prg_memory (rom, 2) & 0xff
                                                   , rom_get_prg_memory (rom, 3) & 0xff
                                                   , rom_get_prg_memory (rom, 4) & 0xff
                                                   , rom_get_prg_memory (rom, 5) & 0xff
                                                   , rom_get_prg_memory (rom, 6) & 0xff
                                                   , rom_get_prg_memory (rom, 7) & 0xff);
  printf ("\trom->prg end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_get_prg_memory (rom, rom_get_prg_size (rom) - 8) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 7) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 6) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 5) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 4) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 3) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 2) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 1) & 0xff);
  printf ("\trom->chr start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_get_chr_memory (rom, 0) & 0xff
                                                   , rom_get_chr_memory (rom, 1) & 0xff
                                                   , rom_get_chr_memory (rom, 2) & 0xff
                                                   , rom_get_chr_memory (rom, 3) & 0xff
                                                   , rom_get_chr_memory (rom, 4) & 0xff
                                                   , rom_get_chr_memory (rom, 5) & 0xff
                                                   , rom_get_chr_memory (rom, 6) & 0xff
                                                   , rom_get_chr_memory (rom, 7) & 0xff);
  printf ("\trom->chr end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_get_prg_memory (rom, rom_get_prg_size (rom) - 8) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 7) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 6) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 5) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 4) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 3) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 2) & 0xff
                                                   , rom_get_prg_memory (rom, rom_get_prg_size (rom) - 1) & 0xff);

  printf ("Load RomNES END\n");

  printf ("Load CPU START\n");

  mapper = mapper_new (rom);

  cpu = cpu_new ();
  /*cpu_set_mapper (cpu, mapper);
  cpu_set_rom (cpu, rom);

  printf ("cpu->mem->[0x0000] = %02x\n", cpu_get_memory (cpu, 0x0000));
  cpu_set_memory (cpu, 0x0000, 0x48);
  printf ("cpu->mem->[0x0000] = %02x\n", cpu_get_memory (cpu, 0x0000));
  printf ("cpu->mem->[0x1000] = %02x\n", cpu_get_memory (cpu, 0x1000));
  cpu_set_memory (cpu, 0x1600, 0x96);
  printf ("cpu->mem->[0x1600] = %02x\n", cpu_get_memory (cpu, 0x1600));
  printf ("cpu->mem->[0x0600] = %02x\n", cpu_get_memory (cpu, 0x0600));

  ADDR16 addr;
  for (addr = 0x8000; addr != 0x8008; ++addr) {
    printf ("\tcpu->mem->[0x%04x] = %02x\n", addr, cpu_get_memory (cpu, addr) & 0xff);
  }

  for (addr = 0xFFF8; addr != 0x0000; ++addr) {
    printf ("\tcpu->mem->[0x%04x] = %02x\n", addr, cpu_get_memory (cpu, addr) & 0xff);
  }*/

  printf ("Load CPU END\n");

  rom_free (rom);
  mapper_free (mapper);
  cpu_free (cpu);

  printf ("Emulator END\n");

  return 0;
}
