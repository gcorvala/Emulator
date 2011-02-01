#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct _Cpu          Cpu;
typedef struct _CpuGB        CpuGB;
typedef struct _BackgroundGB BackgroundGB;
typedef struct _MapGB        MapGB;
typedef struct _RomGB        RomGB;
typedef struct _SpriteGB     SpriteGB;
typedef struct _TileGB       TileGB;
typedef struct {
  //unsigned int : 4;
  unsigned int negative_flag : 1;
  unsigned int half_carry_flag : 1;
  unsigned int carry_flag : 1;
  unsigned int zero_flag : 1;
} CpuGBFlags;

typedef struct _Color     Color;

typedef struct _CpuNES CpuNES;
typedef struct _MapNES MapNES;
typedef struct _RomNES RomNES;

typedef signed char INT8;
typedef signed short int INT16;
typedef signed int INT32;

typedef unsigned char UINT8;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;

typedef UINT8 BYTE;
typedef UINT16 WORD;
typedef UINT32 DWORD;

typedef UINT8 ADDR8;
typedef UINT16 ADDR16;
typedef UINT32 ADDR32;

typedef UINT8 REG8;
typedef union {
  UINT16 r_16;
  struct {
    REG8 l;
    REG8 h;
  } r_8;
} REG16;
typedef union {
  UINT32 r_32;
  struct {
    REG16 l;
    REG16 h;
  } r_16;
} REG32;

typedef int BOOL;

#define FALSE (0)
#define TRUE  (!FALSE)
#define FUNC  __FUNCTION__
#define LINE  __LINE__

#define CPU_GB_FREQ (0x400000)

#endif
