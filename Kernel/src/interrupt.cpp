#include <common.h>
#include <idt.h>

#include <interrupt.h>

void* IRQHandlers[16]{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

extern "C"
void FaultHandler(regs32 *r){
	if(r->int_num < 32){
		switch(r->int_num){
			case 0:
				console::print("Div by 0");
				while(1);
				break;
			default:
				console::print("Unknown Exception!");
				while(1);
				break;
		}
	}
}

extern "C"
void IRQHandler(struct regs32 *r){
	void(*handler)(struct regs32 *r);
	handler = reinterpret_cast<void(*)(struct regs32 *r)>(IRQHandlers[r->int_num-32]);
	if(handler){
		handler(r);
	}

	if(r->int_num >= 40){
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
}

void InstallIRQHandler(int irq, void (*handler)(regs32 *r)){
	IRQHandlers[irq] = (void*)handler;
}

void UninstallIRQHandler(int irq){
	IRQHandlers[irq] = 0;
}

void IRQRemap(){
	outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
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

void InstallIRQs(){
	IRQRemap();
	IDT_SetGate(32, (unsigned)_irq0, 0x08, 0x8E);
	IDT_SetGate(33, (unsigned)_irq1, 0x08, 0x8E);
	IDT_SetGate(34, (unsigned)_irq2, 0x08, 0x8E);
	IDT_SetGate(35, (unsigned)_irq3, 0x08, 0x8E);
	IDT_SetGate(36, (unsigned)_irq4, 0x08, 0x8E);
	IDT_SetGate(37, (unsigned)_irq5, 0x08, 0x8E);
	IDT_SetGate(38, (unsigned)_irq6, 0x08, 0x8E);
	IDT_SetGate(39, (unsigned)_irq7, 0x08, 0x8E);
	IDT_SetGate(40, (unsigned)_irq8, 0x08, 0x8E);
	IDT_SetGate(41, (unsigned)_irq9, 0x08, 0x8E);
	IDT_SetGate(42, (unsigned)_irq10, 0x08, 0x8E);
	IDT_SetGate(43, (unsigned)_irq11, 0x08, 0x8E);
	IDT_SetGate(44, (unsigned)_irq12, 0x08, 0x8E);
	IDT_SetGate(45, (unsigned)_irq13, 0x08, 0x8E);
	IDT_SetGate(46, (unsigned)_irq14, 0x08, 0x8E);
	IDT_SetGate(47, (unsigned)_irq15, 0x08, 0x8E);
}
