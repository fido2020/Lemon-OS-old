#include <stdint.h>
#include <hal.h>
#include <memory.h>

int curX = 0;
int curY = 0;

void putc(char ch, uint8_t colour){
	char* mem = (char*)0xB8000;
	mem [((curY * 80 + curX))*2] = ch;
    mem [((curY * 80 + curX))*2+1] = colour;
	curX++; 

	if(curX >= 80)
	{
		curX = 0;
		curY++;
	}
}

void puts(char* str, uint8_t colour = 0x0F){
	while(*str != 0){
		putc(*str, colour);
		str++;
	}
}

struct memory_map {
	uint32_t	size;
	uint32_t	baseLo;
	uint32_t	baseHi;
	uint32_t	lengthLow,lengthHigh;
	uint32_t	type;
};

struct multiboot_info {
	uint32_t flags;
	uint32_t memoryLo;
	uint32_t memoryHi;
	uint32_t bootDevice;
	uint32_t cmdline;
	uint32_t modsCount;
	uint32_t modsAddr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmapLength;
	uint32_t mmapAddr;
	uint32_t drivesLength;
	uint32_t drivesAddr;
	uint32_t configTable;
	uint32_t bootloaderName;
	uint32_t apmTable;

	uint32_t vbeControlInfo;
	uint32_t vbeModeInfo;
	uint16_t vbeMode;
	uint16_t vbeInterfaceSeg;
	uint16_t vbeInterfaceOff;
	uint16_t vbeInterfaceLen;

	uint64_t framebufferAddr;
	uint32_t framebufferPitch;
	uint32_t framebufferWidth;
	uint32_t framebufferHeight;
	uint8_t framebufferBpp;
	uint8_t framebufferType;
}__attribute__ ((packed));

char* strMemoryTypes[] = {

	"Available",			//memory_region.type==0
	"Reserved",			//memory_region.type==1
	"ACPI Reclaim",		//memory_region.type==2
	"ACPI NVS Memory"		//memory_region.type==3
};

void memmgr_init (size_t memSize, uint32_t bitmap);
void memmgr_init_region (uint32_t base, size_t size);
void memmgr_deinit_region (uint32_t base, size_t size);

extern int kernel_end;

void* malloc (size_t size);

extern "C"
void kmain_early(multiboot_info* mbInfo){
	initialize_hardware();

	uint32_t memSize = 1024 + mbInfo->memoryLo + mbInfo->memoryHi*64;
	memmgr_init (memSize, 0x100000 + (kernel_end-0x100000)*512);

	memory_map* mmap = (memory_map*)mbInfo->mmapAddr;
	while((uint32_t)mmap < mbInfo->mmapAddr + mbInfo->mmapLength) {
		mmap = (memory_map*) ((unsigned int)mmap + mmap->size + sizeof(mmap->size) );
	}

	memmgr_deinit_region();
}
