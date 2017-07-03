#pragma once
#include <stddef.h>
#include <stdint.h>

struct memory_region {
	uint32_t	size;
	uint32_t	baseLo;
	uint32_t	baseHi;
	uint32_t	lengthLow,lengthHigh;
	uint32_t	type;
};

// Memory Manipulation
void* memset(void* dest, int src, size_t count);
void *memcpy(void *dest, const void *src, size_t count);

void* malloc(size_t size);
void* free(void* ptr, size_t size);

// Memory Management
void memmgr_init (size_t memSize, uint32_t bitmap);
void memmgr_init_region (uint32_t base, size_t size);
void memmgr_deinit_region (uint32_t base, size_t size);