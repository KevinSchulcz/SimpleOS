#include "types.h"
#include "util.h"
#include "../driver/port.h"

// Imperecise wait for I/O operations
//  Used only to give peripherals extra time to process a request before another 
//  request is given. This is an issue with old processors which are connected 
//  to devices that aren't specified to the exact cpu timing. Generally not an
//  issue with modern processors now (80486 and after).
void io_wait(void)
{
    port_byte_out(0x80, 0);
}

// Copies nbytes bytes from vsrc to vdest
//  Accounts for dst and src areas overlapping
void *mem_mov(void *vdst, const void *vsrc, uint nbytes)
{
    uint8 *dst       = vdst;
    const uint8 *src = vsrc;

    if (dst < src)
    {
        for (int i = 0; i < nbytes; i++)
        {
            *(dst + i) = *(src + i);
        }
    }
    else
    {
        dst += (nbytes - 1);
        src += (nbytes - 1);
        for (int i = 0; i < nbytes; i++)
        {
            *(dst - i) = *(src - i);
        }
    }

    return vdst;
}

// Copies nbytes bytes from vsrc to vdest
//  (Calls mem_mov under the hood)
void *mem_cpy(void *vdst, const void *vsrc, uint nbytes)
{
    return mem_mov(vdst, vsrc, nbytes);
}

