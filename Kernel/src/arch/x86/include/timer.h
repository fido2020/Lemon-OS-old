#include <idt.h>
#include <stddef.h>

void timer_handler(regs32_t *r);

void timer_register_callback(void(*callback)(uint16_t));

void timer_install();