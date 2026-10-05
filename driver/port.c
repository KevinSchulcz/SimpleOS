#include "../kernel/types.h"

// "=REG" (VAR) -> place value of register into VAR when done
// "REG"  (VAR) -> place value of VAR into register before executing

// Reads and returns 1 byte from the given port
uint8 port_byte_in(uint16 port)
{
    uint8 result = 0;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    
    return result;
}

// Load 1 byte of data into the given port
void port_byte_out(uint16 port, uint8 data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

// Reads and returns 1 word from the given port
uint16 port_word_in(uint16 port)
{
    uint16 result = 0;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    
    return result;
}

// Load 1 word of data into the given port
void port_word_out(uint16 port, uint16 data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}