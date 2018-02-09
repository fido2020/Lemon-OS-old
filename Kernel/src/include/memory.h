#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	unsigned char *memset(uint8_t *src, int c, size_t count);

	void     *malloc(size_t);				//< The standard function.
	void     *realloc(void *, size_t);		//< The standard function.
	void     *calloc(size_t, size_t);		//< The standard function.
	void      free(void *);					//< The standard function.

#ifdef __cplusplus
}
#endif