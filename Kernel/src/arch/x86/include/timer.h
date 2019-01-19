#include <idt.h>
#include <stddef.h>

void timer_handler(regs32_t *r);

void timer_register_callback(void(*callback)(uint16_t));

void timer_install(uint32_t freq);

uint32_t get_uptime_seconds();
uint32_t get_uptime_minutes();
uint32_t get_uptime_hours();