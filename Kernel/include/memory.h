#pragma once

#include <stdtype.h>

#ifdef __cplusplus
	unsigned char *memset(unsigned char *src, int c, size_t count);
	#define CLINK extern "C"
#else
	#define CLINK
#endif

CLINK
void *memset(void *s, int c, size_t count);
CLINK
void *memcpy(void *dest, const void *src, size_t count);
CLINK
void *malloc(uint32_t len);
CLINK
void initMemMgr(size_t _memSize, uint32_t bitmap);
