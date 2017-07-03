#include <stdint.h>
#include <hal.h>
#include <memory.h>
#include <misc.h>

int curX = 0;
int curY = 0;

extern int kernel_base;
extern int kernel_end;

void gfx_init(video_mode_info* videoModeInfo);
void putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void gfx_updatebuffer();

extern "C"
void kmain_early(multiboot_info* mbInfo){
	initialize_hardware();

	uint32_t memSize = 1024 + mbInfo->memoryLo + mbInfo->memoryHi*64;
	memmgr_init (memSize, kernel_base + (kernel_end-kernel_base)*512);

	memory_region* mmap = (memory_region*)mbInfo->mmapAddr;
	while((uint32_t)mmap < mbInfo->mmapAddr + mbInfo->mmapLength) {
		mmap = (memory_region*) ((unsigned int)mmap + mmap->size + sizeof(mmap->size) );
		if(mmap->type == 1 || mmap->type == 0)
			memmgr_init_region(mmap->baseLo,mmap->size);
		//else
		//	memmgr_deinit_region(mmap->baseLo,mmap->size);
	}

	memmgr_deinit_region (0x100000, (kernel_end-kernel_base)*512);

	video_mode_info* vidModeInfo = (video_mode_info*)malloc(sizeof(video_mode_info));
	vidModeInfo->addr = mbInfo->framebufferAddr;
	vidModeInfo->width = mbInfo->framebufferWidth;
	vidModeInfo->height = mbInfo->framebufferHeight;
	vidModeInfo->bpp = mbInfo->framebufferBpp;
	vidModeInfo->pitch = mbInfo->framebufferPitch;

	gfx_init(vidModeInfo);
	for(int i=0;i<100;i++)
		for(int j=0;j<200;j++)
			putpixel(i,j,255,255,0);
	gfx_updatebuffer();
}