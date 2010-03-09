#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu_6502.h"
#include "rom_nes.h"
#include "rom_gb.h"
#include "types.h"

int
main () {
  RomGB *rom_gb;
  /*RomNES *rom;
  CPU6502 *cpu;
  Mapper *mapper;
  ADDR16 addr;
  int i;*/
  char *tmp;

  printf ("Emulator START\n");

  /*printf ("Load RomNES START\n");

  rom = rom_nes_new ("../data/super.nes");

  printf ("\tprg_n_pages = %u\n", rom_nes_get_prg_n_pages (rom));
  printf ("\tprg_size = %lu KB\n", rom_nes_get_prg_size (rom) / 1024);
  printf ("\tchr_n_pages = %u\n", rom_nes_get_chr_n_pages (rom));
  printf ("\tchr_size = %lu KB\n", rom_nes_get_chr_size (rom) / 1024);
  printf ("\tmirroring_flag = %d\n", rom_nes_get_mirroring_flag (rom));
  printf ("\tsram_flag = %d\n", rom_nes_get_sram_flag (rom));
  printf ("\ttrainer_flag = %d\n", rom_nes_get_trainer_flag (rom));
  printf ("\tfour_screen_flag = %d\n", rom_nes_get_four_screen_flag (rom));
  printf ("\tmapper_id = %d\n", rom_nes_get_mapper_id (rom));

  printf ("\trom->prg start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_prg_memory (rom, 0) 
                                                   , rom_nes_get_prg_memory (rom, 1) 
                                                   , rom_nes_get_prg_memory (rom, 2) 
                                                   , rom_nes_get_prg_memory (rom, 3) 
                                                   , rom_nes_get_prg_memory (rom, 4) 
                                                   , rom_nes_get_prg_memory (rom, 5) 
                                                   , rom_nes_get_prg_memory (rom, 6) 
                                                   , rom_nes_get_prg_memory (rom, 7));
  printf ("\trom->prg end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 8) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 7) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 6) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 5) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 4) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 3) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 2) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 1));
  printf ("\trom->chr start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_chr_memory (rom, 0) 
                                                   , rom_nes_get_chr_memory (rom, 1) 
                                                   , rom_nes_get_chr_memory (rom, 2) 
                                                   , rom_nes_get_chr_memory (rom, 3) 
                                                   , rom_nes_get_chr_memory (rom, 4) 
                                                   , rom_nes_get_chr_memory (rom, 5) 
                                                   , rom_nes_get_chr_memory (rom, 6) 
                                                   , rom_nes_get_chr_memory (rom, 7));
  printf (e would like to continue finding new national locations as we spread GNOME throughout Asia, and we are looking for local organizers to rise to the challenge of organizing an excellent GNOME event. The G"\trom->chr end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 8) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 7) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 6) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 5) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 4) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 3) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 2) 
                                                   , rom_nes_get_prg_memory (rom, rom_nes_get_prg_size (rom) - 1));

  printf ("Load RomNES END\n");

  printf ("Load CPU6502 START\n");

  mapper = mapper_new (rom);

  cpu = cpu_6502_new ();
  cpu_6502_set_mapper (cpu, mapper);
  cpu_6502_set_rom (cpu, rom);

  printf ("\tcpu->prg start\n\t\t");
  for (addr = 0x8000; addr != 0x8008; ++addr) {
    if (addr == 0x8004) {
      printf (" ");
    }
    printf ("%02x", cpu_6502_get_memory (cpu, addr) );
  }
  printf ("\n");

  printf ("\tcpu->prg end\n\t\t");
  for (addr = 0xFFF8; addr != 0x0000; ++addr) {
    if (addr == 0xFFFC) {
      printf (" ");
    }
    printf ("%02x", cpu_6502_get_memory (cpu, addr));
  }
  printf ("\n");

  for (i = 0; i < 10; ++i) {
    cpu_6502_step (cpu);
  }

  printf ("Load CPU6502 END\n");

  rom_nes_free (rom);
  mapper_free (mapper);
  cpu_6502_free (cpu);*/

  printf ("Load RomGB START\n");

  rom_gb = rom_gb_new ("../data/gameboy.gb");
  tmp = rom_gb_get_title (rom_gb);
  printf ("\trom_gb_get_title = %s\n", tmp);
  free (tmp);
  printf ("\trom_gb_get_licence = %u\n", rom_gb_get_licence (rom_gb));
  printf ("\trom_gb_get_type = %u\n", rom_gb_get_type (rom_gb));
  printf ("\trom_gb_get_rom_size = %lu KB\n", rom_gb_get_rom_size (rom_gb) / 1024);
  printf ("\trom_gb_get_ram_size = %lu KB\n", rom_gb_get_ram_size (rom_gb) / 1024);
  printf ("\trom_gb_get_version = %u\n", rom_gb_get_version (rom_gb));
  printf ("\trom_gb_has_color_flag = %s\n", rom_gb_has_color_flag (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_has_super_flag = %s\n", rom_gb_has_super_flag (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_is_for_japan = %s\n", rom_gb_is_for_japan (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_check_logo = %s\n", rom_gb_check_logo (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_check_header = %s\n", rom_gb_check_header (rom_gb) ? "TRUE" : "FALSE");
  printf ("\trom_gb_check_full = %s\n", rom_gb_check_full (rom_gb) ? "TRUE" : "FALSE");
  rom_gb_free (rom_gb);

  printf ("Load RomGB END\n");

  printf ("Emulator END\n");

  return 0;
}
