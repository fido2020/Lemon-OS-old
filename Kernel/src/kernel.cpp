#include <lemon.h>

#include <vga.h>
#include <string.h>
#include <stdio.h>


// int32 test
void int32_test()
{
	
	
	/* draw horizontal line from 100,80 to 100,240 in multiple colors
	for(y = 0; y < 200; y++)
		memset((char *)0xA0000 + (y*320+80), y, 160);*/
}


extern "C"
void kmain(){ 
	vga_setMode(Default);
	clearLine(0,0x70);
	setCursorPosition(0,0);
	puts("Lemon v", 0x70);
	puts(lemon_version, 0x70);
	puts("\n$ ~Lemon>", 0x0F);
}