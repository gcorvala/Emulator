#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "cpu_6502.h"
#include "cpu_gb.h"
#include "rom_nes.h"
#include "rom_gb.h"
#include "types.h"

int
main () {
  RomGB *rom_gb;
  CpuGB *cpu_gb;
  RomNES *rom_nes;
  CPU6502 *cpu_nes;
  Mapper *mapper_nes;
  ADDR16 addr;
  int i;
  char *tmp;

  printf ("Test types START\n");

  printf ("\tsizeof (INT8) = %lu bits\n", sizeof (INT8) * 8);
  printf ("\tsizeof (INT16) = %lu bits\n", sizeof (INT16) * 8);
  printf ("\tsizeof (INT32) = %lu bits\n", sizeof (INT32) * 8);
  printf ("\tsizeof (UINT8) = %lu bits\n", sizeof (UINT8) * 8);
  printf ("\tsizeof (UINT16) = %lu bits\n", sizeof (UINT16) * 8);
  printf ("\tsizeof (UINT32) = %lu bits\n", sizeof (UINT32) * 8);
  printf ("\tsizeof (ADDR8) = %lu bits\n", sizeof (ADDR8) * 8);
  printf ("\tsizeof (ADDR16) = %lu bits\n", sizeof (ADDR16) * 8);
  printf ("\tsizeof (ADDR32) = %lu bits\n", sizeof (ADDR32) * 8);
  printf ("\tsizeof (REG8) = %lu bits\n", sizeof (REG8) * 8);
  printf ("\tsizeof (REG16) = %lu bits\n", sizeof (REG16) * 8);
  printf ("\tsizeof (REG32) = %lu bits\n", sizeof (REG32) * 8);

  printf ("\tTRUE = %d\n", TRUE);
  printf ("\tFALSE = %d\n", FALSE);

  printf ("Test types END\n");

  printf ("Emulator START\n");

  printf ("Load RomNES START\n");

  rom_nes = rom_nes_new ("../data/super.nes");

  printf ("\tprg_n_pages = %u\n", rom_nes_get_prg_n_pages (rom_nes));
  printf ("\tprg_size = %lu KB\n", rom_nes_get_prg_size (rom_nes) / 1024);
  printf ("\tchr_n_pages = %u\n", rom_nes_get_chr_n_pages (rom_nes));
  printf ("\tchr_size = %lu KB\n", rom_nes_get_chr_size (rom_nes) / 1024);
  printf ("\tmirroring_flag = %d\n", rom_nes_get_mirroring_flag (rom_nes));
  printf ("\tsram_flag = %d\n", rom_nes_get_sram_flag (rom_nes));
  printf ("\ttrainer_flag = %d\n", rom_nes_get_trainer_flag (rom_nes));
  printf ("\tfour_screen_flag = %d\n", rom_nes_get_four_screen_flag (rom_nes));
  printf ("\tmapper_id = %d\n", rom_nes_get_mapper_id (rom_nes));

  printf ("\trom->prg start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_prg_memory (rom_nes, 0)
                                                   , rom_nes_get_prg_memory (rom_nes, 1)
                                                   , rom_nes_get_prg_memory (rom_nes, 2)
                                                   , rom_nes_get_prg_memory (rom_nes, 3)
                                                   , rom_nes_get_prg_memory (rom_nes, 4)
                                                   , rom_nes_get_prg_memory (rom_nes, 5)
                                                   , rom_nes_get_prg_memory (rom_nes, 6)
                                                   , rom_nes_get_prg_memory (rom_nes, 7));
  printf ("\trom->prg end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 8)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 7)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 6)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 5)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 4)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 3)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 2)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 1));
  printf ("\trom->chr start\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_chr_memory (rom_nes, 0)
                                                   , rom_nes_get_chr_memory (rom_nes, 1)
                                                   , rom_nes_get_chr_memory (rom_nes, 2)
                                                   , rom_nes_get_chr_memory (rom_nes, 3)
                                                   , rom_nes_get_chr_memory (rom_nes, 4)
                                                   , rom_nes_get_chr_memory (rom_nes, 5)
                                                   , rom_nes_get_chr_memory (rom_nes, 6)
                                                   , rom_nes_get_chr_memory (rom_nes, 7));
  printf ("\trom->chr end\n");
  printf ("\t\t%02x%02x%02x%02x %02x%02x%02x%02x\n", rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 8)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 7)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 6)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 5)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 4)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 3)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 2)
                                                   , rom_nes_get_prg_memory (rom_nes, rom_nes_get_prg_size (rom_nes) - 1));

  printf ("Load RomNES END\n");

  printf ("Load CPU6502 START\n");

  mapper_nes = mapper_new (rom_nes);

  cpu_nes = cpu_6502_new ();
  cpu_6502_set_mapper (cpu_nes, mapper_nes);
  cpu_6502_set_rom (cpu_nes, rom_nes);

  printf ("\tcpu->prg start\n\t\t");
  for (addr = 0x8000; addr != 0x8008; ++addr) {
    if (addr == 0x8004) {
      printf (" ");
    }
    printf ("%02x", cpu_6502_get_memory (cpu_nes, addr) );
  }
  printf ("\n");

  printf ("\tcpu->prg end\n\t\t");
  for (addr = 0xFFF8; addr != 0x0000; ++addr) {
    if (addr == 0xFFFC) {
      printf (" ");
    }
    printf ("%02x", cpu_6502_get_memory (cpu_nes, addr));
  }
  printf ("\n");

  for (i = 0; i < 10; ++i) {
    cpu_6502_step (cpu_nes);
  }

  printf ("Load CPU6502 END\n");

  rom_nes_free (rom_nes);
  mapper_free (mapper_nes);
  cpu_6502_free (cpu_nes);

  printf ("Load RomGB START\n");

  rom_gb = rom_gb_new ("../data/gameboy.gb");
  tmp = rom_gb_get_title (rom_gb);
  printf ("\trom_gb_get_title = %s\n", tmp);
  free (tmp);
  printf ("\trom_gb_get_licence = %u\n", rom_gb_get_licence (rom_gb));
  printf ("\trom_gb_get_type = %02x\n", rom_gb_get_type (rom_gb));
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

  printf ("Load CpuGB START\n");

  cpu_gb = cpu_gb_new ();
  cpu_gb_free (cpu_gb);

  printf ("Load CpuGB END\n");

  printf ("Emulator END\n");

  return 0;
}
