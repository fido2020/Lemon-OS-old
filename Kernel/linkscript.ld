OUTPUT_FORMAT(elf32-i386)
ENTRY(entry)

SECTIONS
 {
   . = 100000;
   kernel_base = .;
   .multiboot : {*(.multiboot)}
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
    kernel_end = .;
 }