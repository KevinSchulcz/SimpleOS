#include "types.h"
#include "util.h"
#include "idt.h"
#include "../driver/port.h"
#include "../driver/screen.h"

//  bs_gdt.asm doesnt get linked with the other files
//  it gets prepended after linking with the bootsector
//  so we currently cant access this, find a better solution
//extern uint32 CODE_SEG;         // defined in ../boot-sector/bs_gdt.asm

static idt_gate_t    idt_tab[MAX_INT_ENTRIES];
static idt_tab_ptr_t idt_tab_ptr;               // can this be static? is passed to assembly function

void set_idt_gate(uint8 entry, uint32 base, uint16 seg_sele, uint8 flags)
{
    idt_tab[entry].base_high    = h16(base);
    idt_tab[entry].base_low     = l16(base);
    idt_tab[entry].seg_selector = seg_sele;
    idt_tab[entry].always0      = 0;
    // We will need to use the or expression when implementing user-mode
    idt_tab[entry].flags        = flags; // | 0x60
}

// Loads intel-reserved ISRs, and re-map PIC to use 32P-mode non-reserved
//  idt vectors for IRQs. (need to re-map PIC to go back to 16R-mode)
//  see https://wiki.osdev.org/8259_PIC for more info
void idt_load(void)
{
    uint16 KCS = 0x08;  // kernel code segment?want this to be CODE_SEG see top of file for info
                        // see note on phone for solution i think
    set_idt_gate(0, (uint32)isr0, KCS, 0x8E);
    set_idt_gate(1, (uint32)isr1, KCS, 0x8E);
    set_idt_gate(2, (uint32)isr2, KCS, 0x8E);
    set_idt_gate(3, (uint32)isr3, KCS, 0x8E);
    set_idt_gate(4, (uint32)isr4, KCS, 0x8E);
    set_idt_gate(5, (uint32)isr5, KCS, 0x8E);
    set_idt_gate(6, (uint32)isr6, KCS, 0x8E);
    set_idt_gate(7, (uint32)isr7, KCS, 0x8E);
    set_idt_gate(8, (uint32)isr8, KCS, 0x8E);
    set_idt_gate(9, (uint32)isr9, KCS, 0x8E);
    set_idt_gate(10, (uint32)isr10, KCS, 0x8E);
    set_idt_gate(11, (uint32)isr11, KCS, 0x8E);
    set_idt_gate(12, (uint32)isr12, KCS, 0x8E);
    set_idt_gate(13, (uint32)isr13, KCS, 0x8E);
    set_idt_gate(14, (uint32)isr14, KCS, 0x8E);
    set_idt_gate(15, (uint32)isr15, KCS, 0x8E);
    set_idt_gate(16, (uint32)isr16, KCS, 0x8E);
    set_idt_gate(17, (uint32)isr17, KCS, 0x8E);
    set_idt_gate(18, (uint32)isr18, KCS, 0x8E);
    set_idt_gate(19, (uint32)isr19, KCS, 0x8E);
    set_idt_gate(20, (uint32)isr20, KCS, 0x8E);
    set_idt_gate(21, (uint32)isr21, KCS, 0x8E);
    set_idt_gate(22, (uint32)isr22, KCS, 0x8E);
    set_idt_gate(23, (uint32)isr23, KCS, 0x8E);
    set_idt_gate(24, (uint32)isr24, KCS, 0x8E);
    set_idt_gate(25, (uint32)isr25, KCS, 0x8E);
    set_idt_gate(26, (uint32)isr26, KCS, 0x8E);
    set_idt_gate(27, (uint32)isr27, KCS, 0x8E);
    set_idt_gate(28, (uint32)isr28, KCS, 0x8E);
    set_idt_gate(29, (uint32)isr29, KCS, 0x8E);
    set_idt_gate(30, (uint32)isr30, KCS, 0x8E);
    set_idt_gate(31, (uint32)isr31, KCS, 0x8E);

    // Remap PICs for 32bit protected mode
    // (waits to MAKE SURE PIC processes data see util.c for explination)
    port_byte_out(PICM_COMMAND, ICW1);      // initalize command
    io_wait();
    port_byte_out(PICS_COMMAND, ICW1);
    io_wait();
    port_byte_out(PICM_DATA, ICW2_M);       // set vectors' new offset
    io_wait();
    port_byte_out(PICS_DATA, ICW2_S);
    io_wait();
    port_byte_out(PICM_DATA, 1 << ICW3);    // tells MPIC that there is a SPIC at its IRQ2
    io_wait();
    port_byte_out(PICS_DATA, ICW3);         // tells SPIC its cascade id
    io_wait();
    port_byte_out(PICM_DATA, ICW4);         // make sure PICs use 8086 mode
    io_wait();
    port_byte_out(PICS_DATA, ICW4);
    io_wait();

    // Unmask PICs
    port_byte_out(PICM_DATA, 0);
    port_byte_out(PICS_DATA, 0);

    // Load IRQs into idt
    set_idt_gate(32, (uint32)irq0, KCS, 0x8E);
    set_idt_gate(33, (uint32)irq1, KCS, 0x8E);
    set_idt_gate(34, (uint32)irq2, KCS, 0x8E);
    set_idt_gate(35, (uint32)irq3, KCS, 0x8E);
    set_idt_gate(36, (uint32)irq4, KCS, 0x8E);
    set_idt_gate(37, (uint32)irq5, KCS, 0x8E);
    set_idt_gate(38, (uint32)irq6, KCS, 0x8E);
    set_idt_gate(39, (uint32)irq7, KCS, 0x8E);
    set_idt_gate(40, (uint32)irq8, KCS, 0x8E);
    set_idt_gate(41, (uint32)irq9, KCS, 0x8E);
    set_idt_gate(42, (uint32)irq10, KCS, 0x8E);
    set_idt_gate(43, (uint32)irq11, KCS, 0x8E);
    set_idt_gate(44, (uint32)irq12, KCS, 0x8E);
    set_idt_gate(45, (uint32)irq13, KCS, 0x8E);
    set_idt_gate(46, (uint32)irq14, KCS, 0x8E);
    set_idt_gate(47, (uint32)irq15, KCS, 0x8E);

    // Initalize idt pointer
    idt_tab_ptr.limit = sizeof(idt_gate_t) * MAX_INT_ENTRIES - 1;
    idt_tab_ptr.base  = (uint32)(&idt_tab);
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_tab_ptr)); // unsure about assembly maybe only one %

    println("Re-mapped PIC.");
    println("Set all idt gates.");
}