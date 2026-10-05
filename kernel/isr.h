#ifndef ISR_H
#define ISR_H

#include "types.h"

typedef struct
{
    uint32 ds;                                      // manually pushed
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pusha
    uint32 interrupt_number, error_code;            // pushed by isr#
    uint32 eip, cs, eflags, useresp, ss;            // pushed when int was invoked
} registers_t;

// type for handler functions for custom handlers
typedef void (*isr_t)(registers_t);

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);
void register_int_handler(uint8 int_num, isr_t handler);

#endif