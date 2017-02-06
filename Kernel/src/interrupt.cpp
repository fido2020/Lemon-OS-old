#include <common.h>

#include <interrupt.h>

struct IDTEntry idt[256];
struct IDTPtr idtPtr;

void LoadIDT(){
	__asm__("lidt idtPtr");
}

void IDT_SetGate(uint8_t num, uint64_t offset, uint16_t sel, uint8_t flags){
	idt[num].offset_low = (offset & 0xFFFF);
	idt[num].offset_high = (offset >> 16) & 0xFFFF;
	idt[num].selector = sel;
	idt[num].zero = 0;
	idt[num].flags = flags;
}

void InitIDT(){
	idtPtr.limit = (sizeof(struct IDTEntry)*256)-1;
	idtPtr.offset = (uint32_t)&idt;
	memset(&idt,0,sizeof(struct IDTEntry)*256);
	
	LoadIDT();
}

extern "C"
void FaultHandler(regs32 *r){
	if(r->int_num < 32){
		if(r->int_num < 2){
			console::print("Div by 0");
			while(1);
		}
	}
}

void InstallISRs(){
	IDT_SetGate(0, (unsigned)_isr0,0x08,0x8E);
	IDT_SetGate(1, (unsigned)_isr1,0x08,0x8E);
	IDT_SetGate(2, (unsigned)_isr2,0x08,0x8E);
	IDT_SetGate(3, (unsigned)_isr3,0x08,0x8E);
	IDT_SetGate(4, (unsigned)_isr4,0x08,0x8E);
	IDT_SetGate(5, (unsigned)_isr5,0x08,0x8E);
	IDT_SetGate(6, (unsigned)_isr6,0x08,0x8E);
	IDT_SetGate(7, (unsigned)_isr7,0x08,0x8E);
	IDT_SetGate(8, (unsigned)_isr8,0x08,0x8E);
	IDT_SetGate(9, (unsigned)_isr9,0x08,0x8E);
	IDT_SetGate(10, (unsigned)_isr10,0x08,0x8E);
	IDT_SetGate(11, (unsigned)_isr11,0x08,0x8E);
	IDT_SetGate(12, (unsigned)_isr12,0x08,0x8E);
	IDT_SetGate(13, (unsigned)_isr13,0x08,0x8E);
	IDT_SetGate(14, (unsigned)_isr14,0x08,0x8E);
	IDT_SetGate(15, (unsigned)_isr15,0x08,0x8E);
	IDT_SetGate(16, (unsigned)_isr16,0x08,0x8E);
	IDT_SetGate(17, (unsigned)_isr17,0x08,0x8E);
	IDT_SetGate(18, (unsigned)_isr18,0x08,0x8E);
	IDT_SetGate(19, (unsigned)_isr19,0x08,0x8E);
	IDT_SetGate(20, (unsigned)_isr20,0x08,0x8E);
	IDT_SetGate(21, (unsigned)_isr21,0x08,0x8E);
	IDT_SetGate(22, (unsigned)_isr22,0x08,0x8E);
	IDT_SetGate(23, (unsigned)_isr23,0x08,0x8E);
	IDT_SetGate(24, (unsigned)_isr24,0x08,0x8E);
	IDT_SetGate(25, (unsigned)_isr25,0x08,0x8E);
	IDT_SetGate(26, (unsigned)_isr26,0x08,0x8E);
	IDT_SetGate(27, (unsigned)_isr27,0x08,0x8E);
	IDT_SetGate(28, (unsigned)_isr28,0x08,0x8E);
	IDT_SetGate(29, (unsigned)_isr29,0x08,0x8E);
	IDT_SetGate(30, (unsigned)_isr30,0x08,0x8E);
	IDT_SetGate(31, (unsigned)_isr31,0x08,0x8E);
}