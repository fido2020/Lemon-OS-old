#include <vga.h>



// int32 test
void int32_test()
{
	
	
	/* draw horizontal line from 100,80 to 100,240 in multiple colors
	for(y = 0; y < 200; y++)
		memset((char *)0xA0000 + (y*320+80), y, 160);*/
}


extern "C"
void kmain(){
	/*clear(0x1F);
	puts("Hello World!");*/
	init_vga(Text40x25);
	clear(0x2D);
	/*drawSplash(10,10,44);
	mouse_init();
	while(true){
		mouse_update();
		//drawrect(mouseX,mouseY,2,2,0);
	}*/
}