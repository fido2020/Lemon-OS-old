#pragma once

#define KERNEL_VIRTUAL_BASE 0x0

#include <stdtype.h>

#include <paging.h>
#include <interrupt.h>

extern "C" uint8_t inportb (uint16_t _port);
extern "C" void outportb (uint16_t _port, uint8_t _data);

typedef struct __attribute__ ((packed)) {
	unsigned short di, si, bp, sp, bx, dx, cx, ax;
	unsigned short gs, fs, es, ds, eflags;
} regs16_t;

struct regs32
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_num, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

void InstallISRs();

extern "C" void InstallIRQHandler(int irq, void (*handler)(regs32 *r));
extern "C" void UninstallIRQHandler(int irq);
void IRQRemap();
void InstallIRQs();
extern "C" void IRQHandler(struct regs32 *r);
