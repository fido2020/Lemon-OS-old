#pragma once

#include <stdtype.h>

extern "C"
void *memset(void *s, int c, size_t count);
extern "C"
void *memcpy(void *dest, const void *src, size_t count);
extern "C"
void *malloc(uint32_t len);

unsigned char *memset(unsigned char *src, int c, size_t count);