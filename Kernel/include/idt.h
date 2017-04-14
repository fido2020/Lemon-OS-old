#pragma once

#include <stdtype.h>
#include <memory.h>

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

void loadIDT(IDTPtr idtPtr);
void IDT_SetGate(uint8_t num, uint64_t offset, uint16_t sel, uint8_t flags);
void initIDT();
