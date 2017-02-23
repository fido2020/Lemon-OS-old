 #include <conio.h>
#include <lemon.h>
#include <shell.h>
#include <common.h>
#include <vesadrv.h>
#include <graphics.h>
#include <wmanager.h>
#include <mouse.h>
#include <textscreen.h>
#include <gdt.h>

using namespace console;
using namespace graphics;

#define CODE32SEL 0x08
#define DATA32SEL 0x10

extern gdt_desc_t gdt[5];

extern "C"
void kload()
{
    console::clear();
    console::print("Initializing Lemon...");

    gdtr_t gdtr = { sizeof(gdt_desc_t)*5-1, gdt };

    /* Null descriptor */
    gdt_set_gate(gdt, 0, 0x00000000, 0x00000000, 0x00, 0x00);
    /* 32-bit Code descriptor, flat 4gb */
    gdt_set_gate(gdt, 1, 0x00000000, 0xffffffff, 0x9A, 0xC0);
    /* 32-bit Data descriptor, flat 4gb */
    gdt_set_gate(gdt, 2, 0x00000000, 0xffffffff, 0x92, 0xC0);

    gdt_load(&gdtr, CODE32SEL, DATA32SEL, true);

    InitIDT();

	InstallISRs();
    InstallIRQs();

    __asm__ __volatile__ ("sti");

	Desktop desktop;

	desktop.CreateWindow(10,20,500,250);
	desktop.CreateWindow(300,200,100,200);
	desktop.CreateWindow(200,500,50,50);



	InstallMouseHandler();

	int32_t mouseX;
	int32_t mouseY;

    Vector2 mouseSpd;

    desktop.Paint();

    UpdateScreen();

    fillrect(0,0,200,200,255,0,0);

	/*while(1){
        desktop.Paint();
	    fillrect(mouseX,mouseY,10,10,0);
        mouseSpd = GetMouseSpeed();
        mouseX += mouseSpd.x;
        mouseY += mouseSpd.y;
	}*/
	while(1);
}

void kmain(){
	//PrintProcessList();
	while(1);
}
