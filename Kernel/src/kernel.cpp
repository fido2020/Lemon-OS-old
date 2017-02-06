#include <conio.h>
#include <lemon.h>
#include <shell.h>
#include <common.h>
#include <vesadrv.h>
#include <graphics.h>
#include <wmanager.h>
#include <mouse.h>
#include <textscreen.h>

using namespace console;
using namespace graphics;

int timer_ticks = 0;

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outportb(0x43, 0x36);             /* Set our command byte 0x36 */
    outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler(regs32 *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {
        textscreen::puts("One second has passed\n",0x0F);
    }
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    InstallIRQHandler(0, timer_handler);
}

extern "C"
void kload()
{	
	InitIDT();
	InstallISRs();
	InstallIRQs();
	
	int i = 10/0;
	
	/*Desktop desktop;
	
	desktop.CreateWindow(10,20,500,250);
	desktop.CreateWindow(300,200,100,200);
	desktop.CreateWindow(200,500,50,50);
	
	desktop.Paint();
	
	mouse::Init();
	
	Vector2 mouseSpd = mouse::GetPos();
	
	int32_t mouseX;
	int32_t mouseY;
	
	while(1){
		mouseSpd = mouse::GetPos();
		mouseX = mouseSpd.x;
		mouseY = mouseSpd.y;
		
		fillrect(mouseX,mouseY,10,10,0);
	}*/ timer_install();
	while(1);
}

void kmain(){
	//PrintProcessList();
	while(1);
}