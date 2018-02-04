#include <heap.h>

extern uint32_t kernel_end;
uint32_t next_free_addr = (uint32_t)&kernel_end;

uint32_t kmalloc_internal(uint32_t size) {
	uint32_t temp = next_free_addr;
	next_free_addr += size;
	return temp;
}

uint32_t kmalloc_internal(uint32_t size, int align)
{
	if (align == 1 && (next_free_addr & 0xFFFFF000))
	{
		next_free_addr &= 0xFFFFF000;
		next_free_addr += 0x1000;
	}
	uint32_t tmp = next_free_addr;
	next_free_addr += size;
	return tmp;
}

uint32_t kmalloc_internal(uint32_t size, int align, uint32_t *physical_addr)
{
	if (align == 1 && (next_free_addr & 0xFFFFF000))
	{
		next_free_addr &= 0xFFFFF000;
		next_free_addr += 0x1000;
	}
	if (physical_addr)
	{
		*physical_addr = next_free_addr;
	}
	uint32_t temp = next_free_addr;
	next_free_addr += size;
	return temp;
}

extern "C"
// Page Aligned
uint32_t kmalloc_a(uint32_t size) {
	return kmalloc_internal(size, 1, 0);
}

extern "C"
// Sets a physical address
uint32_t kmalloc_p(uint32_t size, uint32_t *phys_addr) {
	return kmalloc_internal(size, 0, phys_addr);
}

extern "C"
// Page Aligned + Physical Address
uint32_t kmalloc_ap(uint32_t size, uint32_t *phys_addr) {
	return kmalloc_internal(size, 1, phys_addr);
}

extern "C"
uint32_t kmalloc(uint32_t size) {
	return kmalloc_internal(size);
}