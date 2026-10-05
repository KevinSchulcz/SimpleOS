#ifndef TIMER_H
#define TIMER_H

#include "types.h"
#include "isr.h"

#define PIT_FREQ        20     // Times the PIT outputs a signal in Hz
#define PIT_COMMAND     0x43    // PIT command port address
#define PIT_DATA_CH_0   0x40    // PIT data channel 0 port address (for setting freq)
#define PIT_REPEAT_MODE 0x36    // Sets PIT in repeating mode (refreshes divisor counter when it reaches 0)

void init_timer(void);
void timer_callback(registers_t regs);

#endif