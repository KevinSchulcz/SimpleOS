#ifndef IDT_H
#define IDT_H

#include "types.h"

#define MAX_INT_ENTRIES 256         // x86 has exactly 256 interrupts

#define PICM           0x20         // Master PIC base addr
#define PICS           0xA0         // Slave  PIC base addr
#define PICM_COMMAND   PICM         // MPIC command port addr
#define PICM_DATA      (PICM + 1)   // MPIC data    port addr
#define PICS_COMMAND   PICS         // SPIC command port addr
#define PICS_DATA      (PICS + 1)   // SPIC data    port addr
#define ICW1           0x10 | 0x01  // Init command + indicating we will provide ICW4
#define ICW2_M         0x20         // New vector offset for master PIC
#define ICW2_S         0x28         // New vector offset for slave  PIC
#define ICW3           0x02         // Inticates how master/slave is connected for cascading 
#define ICW4           0x01         // Tell PICs to use 8086mode rather than 8080 (has other options)

// Interrupt gate definition
typedef struct
{
    uint16 base_low;            // address of interrupt handler (bits 0-15)
    uint16 seg_selector;        // kernel segment selector
    uint8  always0;             // (should always be set to 0)
    uint8  flags;               // (0-4) constant 00110b; (5-6) privilege lvl called from (DPL); (7) is gate present? (no will cause Interrupt Not Handled exception)
    uint16 base_high;           // address of interrupt handler (bits 16-31)
} __attribute__((packed)) idt_gate_t;

// Holds location of our idt gate vector table
typedef struct
{
    uint16 limit;
    uint32 base;                // Address of first handler in table
} __attribute__((packed)) idt_tab_ptr_t;

void set_idt_gate(uint8 entry, uint32 base, uint16 seg_sele, uint8 flags);
void idt_load(void);

// ISRs reserved for CPU found in interrupt.asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
// IRQs for PICs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif