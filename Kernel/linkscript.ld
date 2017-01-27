/* Old Kernel without Paging * /
OUTPUT_FORMAT(elf32-i386)
ENTRY(entry)
SECTIONS
 {
   . = 100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
 /**/
 
/*Old Kernel with paging* /
ENTRY(entry)
OUTPUT_FORMAT(elf32-i386)
SECTIONS {
   /* The kernel will live at 3GB + 1MB in the virtual
      address space, which will be mapped to 1MB in the
      physical address space. * /
   . = 0xC0100000;

   .text : AT(ADDR(.text) - 0xC0000000) {
       *(.text)
       *(.rodata*)
   }

   .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) {
       *(.data)
   }

   .bss : AT(ADDR(.bss) - 0xC0000000) {
       _sbss = .;
       *(COMMON)
       *(.bss)
       _ebss = .;
   }
}/**/

OUTPUT_FORMAT("elf32-i386");
ENTRY(entry);

/* Multiboot spec uses 0x00100000 as a base */
phys_base = 0x00100000;
/* Offset from bottom of segment, segment set to 0x00 in realmode code */
real_base = 0x00001000;

SECTIONS
{
    . = phys_base;

    /* Place the multiboot record first */
    .multiboot : {
        *(.multiboot);
    }
    __multiboot_secsize = SIZEOF(.multiboot);

    /* This is the tricky part. The LMA (load memory address) is the
     * memory location the code/data is read into memory by the
     * multiboot loader. The LMA is after the colon. We want to tell
     * the linker that the code/data in this section was loaded into
     * RAM in the memory area above 0x100000. On the other hand the
     * VMA (virtual memory address) specified before the colon acts
     * like an ORG directive. The VMA tells the linker to resolve all 
     * subsequent code starting relative to the specified VMA. The
     * VMA in this case is real_base which we defined as 0x1000.
     * 0x1000 is 4KB page aligned (useful if you ever use paging) and
     * resides above the end of the interrupt table and the
     * BIOS Data Area (BDA)
     */
    .realmode real_base : AT(__multiboot_secsize + phys_base) {
        /* The __realmode* values can be used by code to copy
         * the code/data from where it was placed in RAM by the 
         * multiboot loader into lower memory at real_base
         *
         * . (period) is the current VMA */
        __realmode_vma_start = .;

        /* LOADADDR is the LMA of the specified section */ 
        __realmode_lma_start = LOADADDR(.realmode);
        *(.text.realmode);
        *(.data.realmode);
        . = ALIGN(4);
        __realmode_vma_end = .;
    }
    __realmode_secsize   = . - ADDR(.realmode);
    __realmode_secsize_l = __realmode_secsize >> 2;
    __realmode_lma_end   = __realmode_lma_start + __realmode_secsize;

    /* . (period) is the current VMA. We set it to the value that would
     * have been generated had we not changed the VMA in the previous
     * section. The .text section also specified the LMA = VMA with
     * AT(ADDR(.text))
     */
    . = phys_base + __realmode_secsize + __multiboot_secsize;
    .text ALIGN(4K): AT(ADDR(.text)) {
        *(.text);
    }

    /* From this point the linker script is typical */
    .data ALIGN(4K) : {
        *(.data);
        *(.rodata);
    }

    .eh_frame : {
        *(.eh_frame*);
    }

    .bss ALIGN(4K): {
        *(.COMMON);
        *(.bss)
    }

    /* The .note.gnu.build-id section will usually be placed at the beginning
     * of the ELF object. We discard it (if it is present) so that the
     * multiboot header is placed as early as possible in the file. The
     * multiboot header must appear in the first 8K and be on a 4 byte
     * aligned offset per the multiboot spec.
     */
    /DISCARD/ : { 
        *(.note.gnu.build-id);
        *(.comment);
    }
}
