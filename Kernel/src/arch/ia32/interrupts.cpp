#include <system.h>
#include <idt.h>

#include <interrupts.h>

void* irq_handlers[16]{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

extern "C"
void isr_fault_handler(regs32_t *r){
	if(r->int_num < 32){
		switch(r->int_num){
			case 0:
				// Div by 0
				while(1);
				break;
			default:
				// Unknown exception
				while(1);
				break;
		}
	}
}

extern "C"
void irq_handler(regs32_t *r){
	void(*handler)(regs32_t *r);
	handler = reinterpret_cast<void(*)(regs32_t *r)>(irq_handlers[r->int_num-32]);
	if(handler){
		handler(r);
	}

	if(r->int_num >= 40){
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
}

void irq_install_handler(int irq, void (*handler)(regs32_t *r)){
	irq_handlers[irq] = (void*)handler;
}

void irq_uninstall_handler(int irq){
	irq_handlers[irq] = 0;
}

void irq_remap(){
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

void isrs_install(){
	idt_set_gate(0, (unsigned)isr0,0x08,0x8E);
	idt_set_gate(1, (unsigned)isr1,0x08,0x8E);
	idt_set_gate(2, (unsigned)isr2,0x08,0x8E);
	idt_set_gate(3, (unsigned)isr3,0x08,0x8E);
	idt_set_gate(4, (unsigned)isr4,0x08,0x8E);
	idt_set_gate(5, (unsigned)isr5,0x08,0x8E);
	idt_set_gate(6, (unsigned)isr6,0x08,0x8E);
	idt_set_gate(7, (unsigned)isr7,0x08,0x8E);
	idt_set_gate(8, (unsigned)isr8,0x08,0x8E);
	idt_set_gate(9, (unsigned)isr9,0x08,0x8E);
	idt_set_gate(10, (unsigned)isr10,0x08,0x8E);
	idt_set_gate(11, (unsigned)isr11,0x08,0x8E);
	idt_set_gate(12, (unsigned)isr12,0x08,0x8E);
	idt_set_gate(13, (unsigned)isr13,0x08,0x8E);
	idt_set_gate(14, (unsigned)isr14,0x08,0x8E);
	idt_set_gate(15, (unsigned)isr15,0x08,0x8E);
	idt_set_gate(16, (unsigned)isr16,0x08,0x8E);
	idt_set_gate(17, (unsigned)isr17,0x08,0x8E);
	idt_set_gate(18, (unsigned)isr18,0x08,0x8E);
	idt_set_gate(19, (unsigned)isr19,0x08,0x8E);
	idt_set_gate(20, (unsigned)isr20,0x08,0x8E);
	idt_set_gate(21, (unsigned)isr21,0x08,0x8E);
	idt_set_gate(22, (unsigned)isr22,0x08,0x8E);
	idt_set_gate(23, (unsigned)isr23,0x08,0x8E);
	idt_set_gate(24, (unsigned)isr24,0x08,0x8E);
	idt_set_gate(25, (unsigned)isr25,0x08,0x8E);
	idt_set_gate(26, (unsigned)isr26,0x08,0x8E);
	idt_set_gate(27, (unsigned)isr27,0x08,0x8E);
	idt_set_gate(28, (unsigned)isr28,0x08,0x8E);
	idt_set_gate(29, (unsigned)isr29,0x08,0x8E);
	idt_set_gate(30, (unsigned)isr30,0x08,0x8E);
	idt_set_gate(31, (unsigned)isr31,0x08,0x8E);
}

void irqs_install(){
	irq_remap();
	idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}
