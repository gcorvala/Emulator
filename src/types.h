#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char BYTE;
typedef char INT8;
typedef unsigned char UINT8;
typedef short int INT16;
typedef unsigned short int UINT16;
typedef unsigned char ADDR8;
typedef unsigned short int ADDR16;
typedef UINT8 REG8;
typedef union {
  UINT16 r_16;
  struct {
    REG8 l;
    REG8 h;
  } r_8;
} REG16;
typedef int BOOL;

#define FALSE (0)
#define TRUE  (!FALSE)
#define FUNC  __PRETTY_FUNCTION__

#endif
