#pragma once

#include <stdint.h>
#include <system.h>

typedef struct {
	uint16_t base_low;
	uint16_t sel;
	uint8_t null;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_initialize();

typedef void(*isr_t)(regs32_t*);

#ifdef __cplusplus
extern "C"
#endif
void interrupt_register_handler(uint8_t interrupt, isr_t handler);