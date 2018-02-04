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

bool keypending = false;
char key;

extern "C"
void kmain(multiboot_info_t* mb_info){
	init_serial();

	write_serial_string("Initializing Lemon...\n");

	VGA::clearscreen();
	VGA::puts("Initializing Lemon...\n");

	/*video_mode_t video_mode;
	video_mode.address = mb_info->framebufferAddr;
	
	video_mode.width = mb_info->framebufferWidth;
	video_mode.height = mb_info->framebufferHeight;

	video_mode.bpp = mb_info->framebufferBpp;
	video_mode.pitch = mb_info->framebufferPitch;

	video_mode.type = Graphical;

	video_initialize(video_mode);

	console::initialize(video_mode);
	 console::puts("Initializing Lemon...\n\n");*/

	gdt_initialize();
	idt_initialize();

	//paging_initialize();

	//VGA::puts("YAY!\n");

	//page_map(mb_info->framebufferAddr, 0xE0000000, 0);
	//video_mode.address = 0xE000000;

	//video_initialize(video_mode);

	//console::initialize(video_mode);

	//console::puts("Bootloader: ");

	//drawstring("yay!", 0, 50);
	/*console::puts((char*)mb_info->bootloaderName);

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

	//puts("Hello, world!");


	//uint32_t *ptr = (uint32_t*)0xA0000000;
	//uint32_t do_page_fault = *ptr;

	for (;;);
}