#include "timer.h"
#include "types.h"
#include "util.h"
#include "isr.h"
#include "idt.h"
#include "../driver/port.h"
#include "../driver/screen.h"

static uint32 PIT_tick = 0;         // keeps track of which tick PIT is on (only used in timer_callback)

// Initalizes the PIT and sets callback function
void init_timer(void)
{
    // PIT's timer output is sent to IRQ0
    register_int_handler(32, &timer_callback);   // FIXME: we set IRQ0 to 32 prob should define this somewhere

    // To set PIT freq we need to send it a divisor to use.
    //  This PIT's period is 1193180Hz (divisor must be 16bit)
    uint32 PIT_divisor = 1193180 / PIT_FREQ;

    // Tell PIT to go into repeating mode and set divisor
    port_byte_out(PIT_COMMAND, PIT_REPEAT_MODE);
    io_wait();
    port_byte_out(PIT_DATA_CH_0, l8(PIT_divisor));
    io_wait();
    port_byte_out(PIT_DATA_CH_0, h8(PIT_divisor));
    io_wait();

    println("Registered PIT callback handler.");
}

// PIT callback, runs every tick the PIT is set
//  to signal at (TEMP func)
void timer_callback(registers_t regs)
{
    print_at("Tick ", 0, 0);
    print_hex_at(PIT_tick, 0, 5);
    PIT_tick++;
}