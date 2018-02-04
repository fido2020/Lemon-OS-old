#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
#endif
uint32_t kmalloc_a(uint32_t size);							// Page Aligned

#ifdef __cplusplus
extern "C"
#endif
uint32_t kmalloc_p(uint32_t size, uint32_t *phys_addr);		// Returns The Physical Address

#ifdef __cplusplus
extern "C"
#endif
uint32_t kmalloc_ap(uint32_t size, uint32_t *phys_addr);	// Paged Aligned + Physical Address

#ifdef __cplusplus
extern "C"
#endif
uint32_t kmalloc(uint32_t size);							// Normal

typedef struct {
	uint32_t magic;
	uint8_t hole;
	uint32_t size;
} header_t;

typedef struct {
	uint32_t magic;
	header_t *header;
} footer_t;