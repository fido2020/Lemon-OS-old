#include <interrupts.h>

void gdt_install();
void idt_install();

void initialize_hardware(){
    gdt_install();
    idt_install();
    isrs_install();
    irqs_install();
}