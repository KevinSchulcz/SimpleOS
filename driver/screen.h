#ifndef SCREEN_H
#define SCREEN_H

#include "../kernel/types.h"

#define VGA_ADDRESS           0xb8000
#define MAX_ROW               25
#define MAX_COL               80
#define WHITE_ON_BLACK        0x0f

#define SCREEN_REG_CTRL       0x3d4
#define SCREEN_REG_DATA       0x3d5
#define CUR_OFF_HIGH_BYTE_REG 14
#define CUR_OFF_LOW_BYTE_REG  15

void print_at(char *msg, int row, int col);
void print(char *msg);
void println(char *msg);
void print_hex(uint32 num);
void print_hex_at(uint32 num, int row, int col);
void clear_screen(void);

#endif