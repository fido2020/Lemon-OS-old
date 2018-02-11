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
#include <vga.h>
#include <memory.h>
#include <GUI.h>

bool keypending = false;
char key;

extern "C"
void brk() {
	write_serial_string("Breakpoint!");
}

extern "C"
void kmain(uint32_t mb_info_addr){

	multiboot_info_t mb_info;

	init_serial();

	write_serial_string("Initializing Lemon...\n");

	gdt_initialize();
	idt_initialize();

	paging_initialize();

	mb_info = *((multiboot_info_t*)mb_info_addr);

	VGA::puts("yay!");

	/*//map_page(mb_info.framebufferAddr, 0xE0000000);
	
	video_mode_t video_mode;
	uint32_t videoMemoryAddress = /*0xE0000000* / mb_info.framebufferAddr;

	video_mode.width = mb_info.framebufferWidth;
	video_mode.height = mb_info.framebufferHeight;

	video_mode.bpp = mb_info.framebufferBpp;
	video_mode.pitch = mb_info.framebufferPitch;
	video_mode.address = videoMemoryAddress;
	video_mode.type = Graphical;

	video_initialize(video_mode);

	screen_clear(255,0,0);

	GUI gui(&video_mode);

	for (;;) {
		gui.Update();
	}

	//console::initialize(video_mode);
	//console::puts("Initializing Lemon...\n\n");

	/*console::puts("Bootloader: ");
	console::puts((char*)mb_info->bootloaderName);

	console::puts("\n\nVideo Mode");
	console::puts("\n\tWidth: ");
	console::puts(itoa(video_mode.width));
	console::puts("\n\tHeight: ");
	console::puts(itoa(video_mode.height));
	console::puts("\n\tColour Depth: ");
	console::puts(itoa(video_mode.bpp));
	console::puts("bpp");

	keyboard_install();

	for (;;) {
		if (keypending) {
			if (key != 0) {
				console::putc(key);
			}
			keypending = false;
		}
		
	}*/

	for (;;);
}