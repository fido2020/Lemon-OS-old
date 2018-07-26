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
#include <physallocator.h>
#include <multitasking.h>
#include <syscalls.h>
#include <shell.h>
#include <mouse.h>
#include <timer.h>
#include <cpuid.h>
#include <bitmap.h>
#include <snake.h>

extern uint32_t physalloc_used_blocks;

char* buf;

void statusbar_update(uint32_t screen_width) {
	screen_fillrect(0, 0, screen_width, 10, 200, 0, 200);
	drawstring("Lemon OS", 2, 2, 255, 255, 255, 1);
	drawstring("RAM: ~", 72, 2, 255, 255, 255, 1);
	itoa(physalloc_used_blocks * 4 / 8, buf);
	drawstring(buf, 120, 2, 255, 255, 255, 1);
	uint32_t offset = 120 + strlen(buf)*8;
	drawstring("KB Used", offset, 2, 255, 255, 255, 1);
}

char* test_string = "Hello from a syscall!";

void test() {
	screen_clear(255, 255, 0);
	drawstring("YAY!", 0, 0, 0, 0, 0, 24);
	screen_update();
	asm("int $0x69" :: "a"(0), "b"(test_string));
	for (;;);
}

Snake* snekkk;
void Snake_Render_Callback() {
	if(snekkk)
		snekkk->Render();
}

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
	multiboot_module_t resources_module = *((multiboot_module_t*)mb_info.modsAddr);

	// Initialize Paging
	write_serial_string("Setting Up Paging...\r\n");
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
	write_serial_string("Initializing Video Mode\r\n");
	uint32_t vid_mem_size = mb_info.framebufferHeight*mb_info.framebufferPitch;

	map_page(mb_info.framebufferAddr, mb_info.framebufferAddr, vid_mem_size / PAGE_SIZE + 1);
	physalloc_mark_region_used(mb_info.framebufferAddr, vid_mem_size);
	physalloc_mark_region_used(resources_module.mod_start, resources_module.mod_end- resources_module.mod_start);

	// Initialize Video Mode structure
	video_mode_t video_mode;

	video_mode.width = mb_info.framebufferWidth;
	video_mode.height = mb_info.framebufferHeight;
	video_mode.bpp = mb_info.framebufferBpp;
	video_mode.pitch = mb_info.framebufferPitch;
	video_mode.address = mb_info.framebufferAddr;
	video_mode.type = Graphical;

	video_initialize(video_mode);

	cpuid_info_t cpuid_info = cpuid_get_info();

	// Initialize Graphics Driver
	if (mb_info.modsCount < 1) {
		fatal_error("The initrd was not passed in boot config","NO_INITRD"); // Send an error message if the initrd was not passed in boot config
	}
	else if (mem_info.memory_high < 48000) fatal_error("Not enough memory! (< 48MB)", "ERR_NOT_ENOUGH_MEM"); // Throw a fatal if there isnt enough memory at the moment 48MB is demanded as the minimum
	else if (!(cpuid_info.features_edx & CPUID_EDX_SSE2)) fatal_error("CPU does not support SSE2", "ERR_NO_SSE2");


	void* splashscreen_bmp = (void*) (resources_module.mod_start);
	void* progress_bmp = (void*)(resources_module.mod_start + 0x8cd66);
	bitmap_file_header_t bmpfileheader = *((bitmap_file_header_t*)splashscreen_bmp);
	bitmap_info_header_t bmpinfoheader = *((bitmap_info_header_t*)splashscreen_bmp + sizeof(bitmap_file_header_t));
	bitmap_file_header_t bmpfileheader2 = *((bitmap_file_header_t*)progress_bmp);
	bitmap_info_header_t bmpinfoheader2 = *((bitmap_info_header_t*)progress_bmp + sizeof(bitmap_file_header_t));

	screen_clear(0, 0, 0);
	drawbitmap_noscale(video_mode.width / 2 - 260, video_mode.height / 2 - 250, 525, 366, (uint8_t*)(splashscreen_bmp + bmpfileheader.offset), 24);
	screen_update();

	syscalls_init();
	
	for (int i = 0; i < 50; i++) screen_update(); // Loading Bars are nice

	drawbitmap_noscale(video_mode.width / 2 - 32 * 2, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	keyboard_install();
	mouse_install();
	timer_install();

	for (int i = 0; i < 50; i++) screen_update(); // Loading Bars are nice

	drawbitmap_noscale(video_mode.width / 2 - 32 * 1, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	for (int i = 0; i < 50; i++) screen_update(); // Loading Bars are nice
	
	drawbitmap_noscale(video_mode.width / 2, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	WindowManager win_mgr(&video_mode); // Initialize Window Manager

	for (int i = 0; i < 50; i++) screen_update(); // Loading Bars are nice

	drawbitmap_noscale(video_mode.width / 2 + 32 * 1, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	Snake snake(10,34,500,400);
	snekkk = &snake;
	Window* snake_window = win_mgr.Window_New(10, 10, 500, 424, windowtype_gui);
	snake_window->render_callback = Snake_Render_Callback;

	for (;;) {
		win_mgr.Update();
		snake.Relocate(snake_window->x, snake_window->y+24);
		snake.Update();
		//snake.Render();
		screen_update();
	}
}