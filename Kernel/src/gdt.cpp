#include <gdt.h>

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(gdt_desc_t gdt[], const int num, const uint32_t base,
                  const uint32_t limit, const uint8_t access,
                  const uint8_t gran)
{
    /* Setup the descriptor base access */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}
