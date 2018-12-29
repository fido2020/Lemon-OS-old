#pragma once

#include <stddef.h>
#include <stdint.h>

#include <liballoc.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	void* memset(void* src, int c, size_t count);

#ifdef __cplusplus
}
#endif

void *memcpy(void* dest, void* src, size_t count);

void memcpy_sse2(void * dest, void * src, size_t count);