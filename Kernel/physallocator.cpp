#include <physallocator.h>

#include <memory.h>
#include <fatal.h>
#include <paging.h>
#include <serial.h>
#include <string.h>

extern "C" uint32_t kernel_end;

uint32_t mem_bitmap[PHYSALLOC_BITMAP_SIZE_DWORDS];

uint32_t physalloc_used_blocks = 0;
uint32_t physalloc_max_blocks = 0;

// Initialize the physical page allocator
void physalloc_init(memory_info_t* mem_info)
{
	// Throw a fatal if there isnt enough memory at the moment 24MB is demanded as the minimum
	if (mem_info->memory_high < 24576) fatal_error("Not enough memory! (< 24MB)", "ERR_NOT_ENOUGH_MEM");

	memset((uint8_t*)mem_bitmap, 0xFFFFFFFF, PHYSALLOC_BITMAP_SIZE_DWORDS * 4);

	multiboot_memory_map_t* mem_map = mem_info->mem_map;
	multiboot_memory_map_t* mem_map_end = (multiboot_memory_map_t*)(mem_info->mem_map + mem_info->memory_map_len);

	while (mem_map < mem_map_end)
	{
		if (mem_map->type == 1)
			physalloc_mark_region_free(mem_map->base, mem_map->length);
		mem_map = (multiboot_memory_map_t*)((uint32_t)mem_map + mem_map->size + sizeof(mem_map->size));
	}
	physalloc_max_blocks = mem_info->memory_high * 1024 / PHYSALLOC_BLOCK_SIZE;
	physalloc_mark_region_used(0, kernel_end + 0xF00000);
}

// Sets a bit in the physical memory bitmap
inline void physalloc_bit_set(uint32_t bit) {  
	mem_bitmap[bit / 32] |= (1 << (bit % 32));
}

// Clears a bit in the physical memory bitmap
inline void physalloc_bit_clear(uint32_t bit) {
	mem_bitmap[bit / 32] &= ~ (1 << (bit % 32));
}

// Tests a bit in the physical memory bitmap
inline bool physalloc_bit_test(uint32_t bit) {
	return mem_bitmap[bit / 32] & (1 << (bit % 32));
}

// Finds the first free block in physical memory
uint32_t physalloc_first_free_block() {
	for (uint32_t i = 0; i < PHYSALLOC_BITMAP_SIZE_DWORDS; i++)
		if (mem_bitmap[i] != 0xffffffff) // If all 32 bits at the index are used then ignore them
			for (uint32_t j = 0; j < 32; j++) // Test each bit in the dword
				if (!(mem_bitmap[i] & (1 << j)))
					return i * 32 + j;

	// The first block is always reserved
	return 0;
}

// Marks a region in physical memory as being used
void physalloc_mark_region_used(uint32_t base, size_t size) {
	for (uint32_t blocks = size / PHYSALLOC_BLOCK_SIZE, align = base / PHYSALLOC_BLOCK_SIZE; blocks > 0; blocks--, physalloc_used_blocks++)
		physalloc_bit_set(align++);
}

// Marks a region in physical memory as being free
void physalloc_mark_region_free(uint32_t base, size_t size) {
	for (uint32_t blocks = size / PHYSALLOC_BLOCK_SIZE, align = base / PHYSALLOC_BLOCK_SIZE; blocks > 0; blocks--, physalloc_used_blocks++)
		physalloc_bit_clear(align++);
}

// Allocates a block of physical memory
uint32_t physalloc_alloc_block() {
	uint32_t index = physalloc_first_free_block();
	if (index == 0)
		return 0;

	physalloc_bit_set(index);
	physalloc_used_blocks++;

	return index * PHYSALLOC_BLOCK_SIZE;
}

// Frees a block of physical memory
void physalloc_free_block(uint32_t addr) {
	uint32_t index = addr / PHYSALLOC_BLOCK_SIZE;
	physalloc_bit_clear(index);
	physalloc_used_blocks--;
}