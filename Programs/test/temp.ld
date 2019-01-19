ENTRY(main)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	. = 100000;
	
    .multiboot : { *(.multiboot) }
	.text : { *(.text) }
	.data : {
		*(.data) 
		*(.rodata*)
	}
	.bss  : { *(.bss)  }
	/DISCARD/ : {
       *(.eh_frame);
       *(.comment*);
   }
    kernel_end = .;
}