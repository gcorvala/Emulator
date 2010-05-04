#include "utils.h"

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int
printf_asm (ADDR32 addr, UINT32 opcode, const char *assembly, const char *format, ...) {
  va_list args;
  char *str = NULL;
  int res;

  va_start (args, format);
  res = vasprintf (&str, format, args);
  va_end (args);

  res = printf ("[0x%08X]  [%8X]  [%s]  [%s]\n", addr, opcode, str, assembly);
  free (str);

  return res;
}
