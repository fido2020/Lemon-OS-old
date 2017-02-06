#pragma once

#define KERNEL_VIRTUAL_BASE 0x0

#include <paging.h>


#include <stdtype.h>


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

struct IDTEntry{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t offset_high;
}__attribute__((packed));

struct IDTPtr{
	uint16_t limit;
	uint32_t offset;
}__attribute__((packed));
	
extern "C"
void LoadIDT(IDTPtr idtPtr);

extern "C"
void IDT_SetGate(uint8_t num, uint64_t offset, uint16_t sel, uint8_t flags);

extern "C"
void InitIDT();

void InstallISRs();

extern "C" void InstallIRQHandler(int irq, void (*handler)(regs32 *r));
extern "C" void UninstallIRQHandler(int irq);
void IRQRemap();
void InstallIRQs();
extern "C" void IRQHandler(struct regs32 *r);