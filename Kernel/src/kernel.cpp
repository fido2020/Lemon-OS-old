#include <conio.h>
#include <lemon.h>
#include <shell.h>
#include <common.h>
#include <vesadrv.h>
#include <graphics.h>
#include <wmanager.h>

using namespace console;
using namespace graphics;

extern "C"
void kload()
{	
	/*InitPaging();
	InitMultitasking();
	
	clear();
	print(Lemon::splash);
	panic("ERR_TASKING_FAIL","Tasking has failed and was not setup. System will now halt.", false);
	*/
	
	//vesa_mode_info_t *modeInfo = init();
	
	//fillrect(128,240,50,40,255,255,0);
	
	Desktop desktop;
	
	desktop.CreateWindow(10,20,500,250);
	desktop.CreateWindow(300,200,100,200);
	desktop.CreateWindow(200,500,50,50);
	
	desktop.Paint();
}

void kmain(){
	//PrintProcessList();
	while(1);
}