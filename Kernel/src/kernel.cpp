#include <gdt.h>
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
#include <bitmap.h>

extern "C"
void kmain(uint32_t mb_info_addr){
	 
	multiboot_info_t mb_info;

	// Initialize GDT and IDT
	gdt_initialize();
	idt_initialize();

	mb_info = *((multiboot_info_t*)mb_info_addr);
	multiboot_module_t initrd_module = *((multiboot_module_t*)mb_info.modsAddr);

	

	// Initialize Paging
	paging_initialize();

	// Initialize serial port for debugging
	init_serial();
	write_serial_string("Initializing Lemon...\r\n"); 

	map_page(mb_info.mmapAddr, mb_info.mmapAddr, mb_info.mmapLength / PAGE_SIZE + 1);
	multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)mb_info.mmapAddr;


	for (int i = 0; i < mb_info.mmapLength; i += memory_map->size + sizeof(memory_map->size)) {
		memory_map += i;
		if (memory_map->type != 1) {
			mark_pages_reserved(memory_map->base, memory_map->base, memory_map->length / PAGE_SIZE + 1);
		}
	}

	// Map Video Memory
	uint32_t vid_mem_size = mb_info.framebufferHeight*mb_info.framebufferPitch;
	map_page(mb_info.framebufferAddr, mb_info.framebufferAddr, vid_mem_size/PAGE_SIZE + 1);

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

	// Send an error message if the initrd was not passed in boot config
	if (mb_info.modsCount < 1) {
		fatal_error("The initrd was not passed in boot config","NO_INITRD");
	}

	// Initialize initrd
	initrd_init(initrd_module.mod_start, initrd_module.mod_end - initrd_module.mod_start);

	lemoninitfs_node_t** initrd_fs_nodes = initrd_list();
	lemoninitfs_header_t* initrd_fs_header = initrd_get_header();

	/*for (int i = 0; i < 16; i++) {
		lemoninitfs_node_t node = (*initrd_fs_nodes)[i];
		char* fname = (char*)malloc(10);
		substr(9, node.filename, fname);
		if (fname == "Lemon.bmp") {
			uint32_t buffer = (uint32_t)malloc(node.size+1);
			initrd_read(node, (uint8_t*)buffer);
			bitmap_header_t* hdr = (bitmap_header_t*)buffer;
			write_serial_string(hdr->magic);
			free((void*)buffer);
		}
		free(fname);
	}*/

	// Initialize Window Manager
	WindowManager wman(&video_mode);

	// Initialize Console
	/*Console console(video_mode,0,0,mb_info.framebufferWidth,mb_info.framebufferHeight);

	console.clear(0, 0, 0);

	console.puts("RAM: ");
	console.puts(itoa((mb_info.memoryLo + mb_info.memoryHi) / 1024));
	console.puts(" MB\n\n");*/

	for (;;) {
		//shell.Update();
		//console.refresh();
		wman.Update();
		screen_update();
	}

	for (;;);
}