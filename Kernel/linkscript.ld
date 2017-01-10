/* Old Kernel without Paging */
OUTPUT_FORMAT(binary)
ENTRY(entry)
SECTIONS
 {
 
   . = 100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
}