ENTRY(entry)
OUTPUT_FORMAT(elf32-i386)

KERNEL_VIRTUAL_BASE = 0xC0000000;

SECTIONS {
   /* The multiboot data and code will exist in low memory
      starting at 0x100000 */

   . = 0x00100000;
   .multiboot.hdr : {
		*(.multiboot.hdr)
   }

   .multiboot.text : {
		*(.multiboot.text)
   }

   .multiboot.data : {
       *(.multiboot.data)
   }

   

   /* The kernel will live at 3GB + 1MB in the virtual
      address space, which will be mapped to 1MB in the
      physical address space. */

   . += KERNEL_VIRTUAL_BASE;
   .text ALIGN(4096) : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE) {
       *(.text)
   }

   .data ALIGN (4096) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE) {
       *(.data)
       *(.rodata*)
   }

   .bss ALIGN (4096) : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE) {
       _sbss = .;
       *(COMMON)
       *(.bss)
       _ebss = .;
   }

   /DISCARD/ : {
       *(.eh_frame);
       *(.comment*);
   }

   kernel_end = . - KERNEL_VIRTUAL_BASE;
}


/*SECTIONS
{
   . = 100000;
   kernel_base = .;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
    kernel_end = .;
}*/