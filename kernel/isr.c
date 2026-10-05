#include "types.h"
#include "isr.h"
#include "../driver/screen.h"
#include "../driver/port.h"

isr_t int_handlers[256];     // x86 has 256 interrupts

// Called from interrupt.asm isr common handler
void isr_handler(registers_t regs)
{
    print("Interrupt: ");
    print_hex(regs.interrupt_number);
    print("\n");
}

// Called from interrupt.asm irq common handler
void irq_handler(registers_t regs)
{
    // We need to send EOI to PIC so it can regain control, if slave sent
    //  the IRQ we need to send EOI to it, and we need to send EOI to master
    //  PIC always
    //  (might want to make this into its own function)
    if (regs.interrupt_number > 39) 
        port_byte_out(0xA0, 0x20);
    port_byte_out(0x20, 0x20); 

    if (int_handlers[regs.interrupt_number] != 0)
    {
        print("Interrupt - calling IRQ handler: ");
        print_hex(regs.interrupt_number);
        print("\n");
        isr_t handl = int_handlers[regs.interrupt_number];
        // handl(regs);         // FIXME: after this line runs we get Invalid opcode interrupt
    }
    else
    {
        print("Interrupt: ");
        print_hex(regs.interrupt_number);
        print(" [WARNING] no handler found.");
        print("\n");
    }
}

// Sets handler for ISR int_num
void register_int_handler(uint8 int_num, isr_t handler)
{
    int_handlers[int_num] = handler;
}