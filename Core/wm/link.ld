SECTIONS
{
	.text : { *(.text); }
	.data : {
		*(.data) ;
		*(.rodata*);
	}
	.bss  : { *(.bss);}

	/DISCARD/ : {
       *(.eh_frame);
       *(.comment*);
   }
    
    kernel_end = .; 
} 
