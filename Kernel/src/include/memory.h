#pragma once

#include <stddef.h>
#include <stdint.h>

#include <liballoc.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	unsigned char *memset(uint8_t *src, int c, size_t count);

#ifdef __cplusplus
}
#endif

void *memcpy(void* dest, void* src, size_t count);

void* sse2_memcpy(void* dest, void* src, size_t count);