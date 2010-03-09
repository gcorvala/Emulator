#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char BYTE;

typedef signed char INT8;
typedef unsigned char UINT8;

typedef signed short int INT16;
typedef unsigned short int UINT16;

typedef signed int INT32;
typedef unsigned int UINT32;

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
#define FUNC  __PRETTY_FUNCTION__

#endif
