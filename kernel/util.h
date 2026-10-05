#ifndef UTIL_H
#define UTIL_H

#include "types.h"

void io_wait(void);

void *mem_cpy(void *vdst, const void *vsrc, uint nbytes);
void *mem_mov(void *vdst, const void *vsrc, uint nbytes);

#endif