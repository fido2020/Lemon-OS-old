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
#include <task.h>

using namespace console;
using namespace graphics;

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

    InitIDT();

	InstallISRs();
    InstallIRQs();

    uint32_t memSize = 1024 + mbInfo->memoryLo + mbInfo->memoryHi;

    InitMemMgr(memSize,(phys_base - kernel_end)*512 + 0x100000);


    print("\nMemory: ");
    print(itoa(memSize,nullptr,10));
    print(" (");
    print(itoa(memSize/1024,nullptr,10));
    print(" MB)\n");

	//InitMultitasking();
	initGfx();

    while(1);
}

void kmain(){
	//PrintProcessList();
	print("Yay it worked! Loading test task\n");
	//process_t* p = CreateProcess("TestTask",(uint32_t)test);
	//AddProcess(p);
	while(1);
}
