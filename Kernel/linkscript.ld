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

	kernel_end = .;

    /DISCARD/ : {
        *(.note.gnu.build-id);
        *(.comment);
    }
}

/*
OUTPUT_FORMAT("elf32-i386");
ENTRY(entry);

/* Multiboot spec uses 0x00100000 as a base * /
phys_base = 0x00100000;
/* Offset from bottom of segment, segment set to 0x00 in realmode code * /
real_base = 0x00001000;

SECTIONS
{
    . = 0xC0100000;

    /* Place the multiboot record first * /
    .multiboot ALIGN(4K): AT(ADDR(.multiboot) - 0xC0000000){
        *(.multiboot);
    }
    __multiboot_secsize = SIZEOF(.multiboot);

    .realmode real_base : AT(__multiboot_secsize + phys_base) {
        /* The __realmode* values can be used by code to copy
         * the code/data from where it was placed in RAM by the
         * multiboot loader into lower memory at real_base
         *
         * . (period) is the current VMA * /
        __realmode_vma_start = .;

        /* LOADADDR is the LMA of the specified section * /
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
     * /
    . = phys_base + __realmode_secsize + __multiboot_secsize + 0xC0000000;
    .text ALIGN(4K): AT(ADDR(.text) - 0xC0000000) {
        *(.text);
    }

    /* From this point the linker script is typical * /
    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000){
        *(.data);
        *(.rodata);
    }

    .eh_frame : AT(ADDR(.eh_frame) - 0xC0000000){
        *(.eh_frame*);
    }

    .bss ALIGN(4K): AT(ADDR(.bss) - 0xC0000000){
        *(.COMMON);
        *(.bss)
    }

	kernel_end = . - 0xC0000000;

    /DISCARD/ : {
        *(.note.gnu.build-id);
        *(.comment);
    }
}
*/
