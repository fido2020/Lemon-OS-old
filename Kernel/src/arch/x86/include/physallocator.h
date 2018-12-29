#pragma once

#include <stdint.h>
#include <mischdr.h>
#include <stddef.h>

// The size of a block in phyiscal memory
#define PHYSALLOC_BLOCK_SIZE 4096
// The amount of blocks in a byte
#define PHYSALLOC_BLOCKS_PER_BYTE 8

// The size of the memory bitmap in dwords
#define PHYSALLOC_BITMAP_SIZE_DWORDS 32768

void physalloc_init(memory_info_t* mem_info);
void physalloc_mark_region_used(uint32_t base, size_t size);
void physalloc_mark_region_free(uint32_t base, size_t size);
uint32_t physalloc_alloc_block();
