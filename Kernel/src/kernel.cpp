#include  <gdt.h>
#include <idt.h>
#include <fatal.h>
#include <paging.h>
#include <video.h>
#include <mischdr.h>
#include <string.h>
#include <console.h>
#include <keyboard.h>
#include <serial.h>
#include <initrd.h>
#include <memory.h>
#include <windowmanager.h>
#include <math.h>
#include <physallocator.h>
#include <bitmap.h>

extern "C"
void kmain(uint32_t mb_info_addr){
	multiboot_info_t mb_info;

	// Initialize serial port for debugging
	init_serial();
	write_serial_string("Initializing Lemon...\r\n");

	// Initialize GDT and IDT
	gdt_initialize();
	idt_initialize();

	mb_info = *((multiboot_info_t*)mb_info_addr);
	multiboot_module_t initrd_module = *((multiboot_module_t*)mb_info.modsAddr);

	

	// Initialize Paging
	paging_initialize();

	map_page(mb_info.mmapAddr, mb_info.mmapAddr, mb_info.mmapLength / PAGE_SIZE + 1);
	multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)mb_info.mmapAddr;

	memory_info_t mem_info;
	mem_info.memory_high = mb_info.memoryHi;
	mem_info.memory_low = mb_info.memoryLo;
	mem_info.mem_map = memory_map;
	mem_info.memory_map_len = mb_info.mmapLength;

	//Initialize Physical Page Allocator
	physalloc_init(&mem_info);

	// Map Video Memory
	uint32_t vid_mem_size = mb_info.framebufferHeight*mb_info.framebufferPitch;

	map_page(mb_info.framebufferAddr, mb_info.framebufferAddr, vid_mem_size / PAGE_SIZE + 1);

	// Initialize Video Mode structure
	video_mode_t video_mode;

	video_mode.width = mb_info.framebufferWidth;
	video_mode.height = mb_info.framebufferHeight;
	video_mode.bpp = mb_info.framebufferBpp;
	video_mode.pitch = mb_info.framebufferPitch;
	video_mode.address = mb_info.framebufferAddr;
	video_mode.type = Graphical;

	// Initialize Graphics Driver
	video_initialize(video_mode);

	screen_clear_direct(0, 0, 0);

	// Send an error message if the initrd was not passed in boot config
	if (mb_info.modsCount < 1) {
		fatal_error("The initrd was not passed in boot config","NO_INITRD");
	}
	
	// Initialize initrd
	initrd_init(initrd_module.mod_start, initrd_module.mod_end - initrd_module.mod_start);

	lemoninitfs_node_t** initrd_fs_nodes = initrd_list();
	lemoninitfs_header_t* initrd_fs_header = initrd_get_header();

	bitmap_file_header_t* bmpfileheader = (bitmap_file_header_t*)(initrd_fs_nodes[0]->offset + initrd_module.mod_start);
	bitmap_info_header_t* bmpinfoheader = (bitmap_info_header_t*)(initrd_fs_nodes[0]->offset + 14 + initrd_module.mod_start);

	write_serial_string("\r\n");
	write_serial_string(itoa(bmpfileheader->size));
	write_serial_string("\r\n");
	write_serial_string(itoa(bmpfileheader->offset));

	uint8_t* vidmem = (uint8_t*)video_mode.address;
	uint8_t* bmpdata = (uint8_t*)(initrd_fs_nodes[0]->offset + initrd_module.mod_start + 54);

	//drawbitmap(0, 0, bmpinfoheader->width, bmpinfoheader->height, bmpdata, 24);

	// Initialize Window Manager
	WindowManager wman(&video_mode);

	for (;;) {
		//shell.Update();
		//console.refresh();
		wman.Update();
		screen_update();
	}

	for (;;);
}