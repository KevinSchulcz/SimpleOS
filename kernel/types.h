#ifndef TYPES_H
#define TYPES_H

// Short hands
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

// Sized-names
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;  // ints not guarenteed to be 32bits only >=16

// Bit manipulation
#define l8(num)  (uint8)((num) & 0xFF)
#define h8(num)  (uint8)(((num) >> 8) & 0xFF)
#define l16(num) (uint16)((num) & 0xFFFF)
#define h16(num) (uint16)(((num) >> 16) & 0xFFFF)

#endif