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

bool keypending = false;
char key;

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

	// Initialize Window Manager
	WindowManager wman(&video_mode);

	// Initialize Console
	Console console(video_mode,20,20,400,300);

	console.clear(0, 0, 0);
	console.puts("Initrd Contents:\n", 255, 255, 255);

	lemoninitfs_node_t node;

	// Display initrd contents in console
	for (int i = 0; i < initrd_fs_header->num_files; i++) {
		console.puts((*initrd_fs_nodes)[i].filename, 255, 255, 255);
		console.puts(";  Size:");
		console.puts(itoa(((*initrd_fs_nodes)[i].size)/1024), 255, 255, 255);
		console.puts(" KB\n");
		node = (*initrd_fs_nodes)[i];
		if (node.size / 1024 < 2) {
			console.puts(node.filename);
			console.puts(" contents:\n");
			char* buffer = (char*)malloc(node.size);
			initrd_read(node, (uint8_t*)buffer);
			console.puts(buffer);
			console.putc('\n');
		}

	}

	screen_update();

	for (;;) {
		wman.Update();
		console.refresh();
		screen_update();
	}

	for (;;);
}