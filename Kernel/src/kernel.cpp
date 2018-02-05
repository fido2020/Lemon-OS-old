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
	write_serial(mb_info->framebufferAddr >> 24);
	write_serial(mb_info->framebufferAddr >> 16 & 255);
	write_serial(mb_info->framebufferAddr >> 8 & 255);

	gdt_initialize();
	idt_initialize();

	paging_initialize();

	//VGA::puts("YAY!\n");

	map_page(mb_info->framebufferAddr, 0xCC000000);
	
	video_mode_t video_mode;

	video_mode.width = mb_info->framebufferWidth;
	video_mode.height = mb_info->framebufferHeight;

	video_mode.bpp = mb_info->framebufferBpp;
	video_mode.pitch = mb_info->framebufferPitch;
	video_mode.address = 0xCC000000;
	video_mode.type = Graphical;

	video_initialize(video_mode);

	screen_clear(255, 255, 255);

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

	//puts("Hello, world!");


	//uint32_t *ptr = (uint32_t*)0xA0000000;
	//uint32_t do_page_fault = *ptr;

	for (;;);
}