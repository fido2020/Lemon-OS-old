#pragma once

#define KERNEL_VIRTUAL_BASE 0x0

#include <stdtype.h>
#include <paging.h>

extern uint8_t inportb (uint16_t _port);
extern void outportb (uint16_t _port, uint8_t _data);

typedef struct __attribute__ ((packed)) {
	unsigned short di, si, bp, sp, bx, dx, cx, ax;
	unsigned short gs, fs, es, ds, eflags;
} regs16_t;


extern "C"
void int32(unsigned char intnum, regs16_t *regs);