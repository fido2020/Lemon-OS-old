#include <system.h>
#include <memory.h>
// int32 test

/*void VbeSetMode (int mode) {
   /* call BIOS * /
   regs16_t regs;
   regs.ax = 0x12;
   regs.bx = mode;
   int32 (0x10, &regs);
}*/

extern "C"
void kmain()
{
	regs16_t regs;
	regs.ax = 0x13;
	int32(0x10,&regs);
	
	while(1){}
}

