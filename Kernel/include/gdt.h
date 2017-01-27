#pragma once

#include <stdtype.h>

typedef struct
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed)) gdt_desc_t;

typedef struct {
    uint16_t limit;
    gdt_desc_t *gdt;
} __attribute__((packed)) gdtr_t;

typedef struct {
    void *ptr;
    uint32_t sel;
} far_ptr_t;

extern void gdt_set_gate(gdt_desc_t gdt[], const int num, const uint32_t base,
                         const uint32_t limit, const uint8_t access,
                         const uint8_t gran);

static inline void gdt_load(gdtr_t * const gdtr, const uint16_t codesel,
                            const uint16_t datasel, const bool flush)
{
    far_ptr_t far_ptr;

    /* Dummy variable used as a fake dependency to avoid optimization
     * reordering of the inline assembly. The flush (if requested) must
     * always come after we set the GDT, not before */
    int dummy;

    /* load the GDT register */
    __asm__ __volatile__ ("lgdt %[gdtr]"
                          : "=X"(dummy)
                          : [gdtr]"m"(*gdtr));

    /* This flushes the selector registers to ensure the new
     * descriptors are used. */
    if (flush) {
        /* The indirect absolute jump is because we can't
         * assume that codesel is an immediate value
         * as it may be passed in a register. We build a
         * far pointer in memory and indirectly jump through
         * that pointer. This explicitly sets CS selector */
        __asm__  __volatile__ (
                 "movl %[codesel], %[sel]\n\t"
                 "movl $1f, %[ptr]\n\t"
                 "ljmpl *%[ptr]\n"
                 "1:\n\t"
                 "mov %[datasel], %%ds\n\t"
                 "mov %[datasel], %%es\n\t"
                 "mov %[datasel], %%ss\n\t"
                 "mov %[datasel], %%fs\n\t"
                 "mov %[datasel], %%gs"
                 : [ptr]"=m"(far_ptr.ptr),
                   [sel]"=m"(far_ptr.sel)
                 : "X"(dummy),
                   [datasel]"r"(datasel),
                   [codesel]"ri"((uint32_t)codesel));
    }
    return;
}