#include <gdt.h>
#include <stdint.h>

gdt_entry_t gdt[5];
gdt_ptr_t gdt_ptr;

extern "C" void gdt_flush(uint32_t ptr);

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
	gdt[num].base_high = (base >> 24) & 0xFF;
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_low = (base & 0xFFFF);

	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = (limit >> 16) & 0x0F;

	gdt[num].granularity |= granularity & 0xF0;
	gdt[num].access = access;
}

void gdt_initialize() {
	gdt_ptr.limit = sizeof(gdt_entry_t) * 5 - 1;
	gdt_ptr.base = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);				// Null seg
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code seg
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data seg
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code seg
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data seg

	gdt_flush((uint32_t)&gdt_ptr);				// Flush/Reload the GDT
}
