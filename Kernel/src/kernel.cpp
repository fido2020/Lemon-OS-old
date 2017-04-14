#include <conio.h>
#include <console.h>
#include <lemon.h>
#include <shell.h>
#include <common.h>
#include <vesadrv.h>
#include <graphics.h>
#include <wmanager.h>
#include <mouse.h>
#include <textscreen.h>
#include <gdt.h>
#include <task.h>

using namespace console;
using namespace Graphics;

#define CODE32SEL 0x08
#define DATA32SEL 0x10

extern gdt_desc_t gdt[5];

struct multiboot_info {

	uint32_t	flags;
	uint32_t	memoryLo;
    uint32_t    memoryHi;
	uint32_t	bootDevice;
	uint32_t	cmdLine;
	uint32_t	modsCount;
	uint32_t	modsAddr;
	uint32_t	syms0;
	uint32_t	syms1;
	uint32_t	syms2;
	uint32_t	mmapLen;
	uint32_t	mmapAddr;
	uint32_t	drivesLen;
	uint32_t	drivesAddr;
	uint32_t	configTable;
	uint32_t	bootloaderName;
	uint32_t	apmTable;
	uint32_t	vbeControlInfo;
	uint32_t	vbeModeInfo;
	uint16_t	vbeMode;
	uint32_t	vbeInterfaceAddr;
	uint16_t	vbeInterfaceLen;
};

extern int32_t kernel_end;

extern int32_t phys_base;

uint32_t physMemSize;

extern "C"
void kload(multiboot_info* mbInfo)
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

    initIDT();

	installISRs();
    installIRQs();

    physMemSize = 1024 + mbInfo->memoryLo + mbInfo->memoryHi;

    initMemMgr(physMemSize,(phys_base - kernel_end)*512 + 0x100000);


    print("\nMemory: ");
    print(itoa(physMemSize,nullptr,10));
    print(" (");
    print(itoa(physMemSize/1024,nullptr,10));
    print(" MB)\n");

	initMultitasking();

    while(1);
}

void kmain(){
	vesa_mode_info_t* gfxModeInfo = initGfx();

	drawstring("Lemon v",10,10,220,220,40,2);
	drawstring(Lemon::version,122,10,220,220,40,2);

	Console shell(20,36,gfxModeInfo->width-40,gfxModeInfo->height-56,1);
	shell.print("Lemon Kernel Mode\n");
	shell.print("RAM: ");
	shell.printNum(physMemSize/1024);
	shell.print("MB\n");
	shell.print("Current Video Mode: ");
	shell.printNum(gfxModeInfo->width);
	shell.print('x');
	shell.printNum(gfxModeInfo->height);
	shell.print(' ');
	shell.printNum(gfxModeInfo->bpp);
	shell.print("bpp\n");

	for(int i=0;i<177;i++)
		shell.print("a\n");

	//shell.clear();

	while(1);
}
