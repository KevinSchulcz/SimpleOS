#include "types.h"
#include "../driver/screen.h"
#include "util.h"
#include "idt.h"
#include "timer.h"

void main()
{
    // clear_screen();                  // Clears bootloader logs

    println("Loaded into SimpleOS");
    println("\n");

    idt_load();
    
    // Need to allow interrupts from PIC
    __asm__ __volatile__("sti");        // COMMENT THIS LINE to stop timer log spam
    init_timer();

    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $0x4");
}