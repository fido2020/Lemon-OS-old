#include <conio.h>
#include <lemon.h>
#include <shell.h>
#include <common.h>
#include <vesadrv.h>

using namespace console;


extern "C"
void kload()
{	
	/*InitPaging();
	InitMultitasking();
	
	clear();
	print(Lemon::splash);
	panic("ERR_TASKING_FAIL","Tasking has failed and was not setup. System will now halt.", false);
	*/
	
	vesa_mode_info_t *GfxInfo = EnterGraphicsMode();
	
	putpixel((unsigned char*)GfxInfo->physbase,0,0,0x7800);
}

void kmain(){
	//PrintProcessList();
	while(1);
}