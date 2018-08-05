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
cpuid_info_t cpuid_info;

char* buf;
char* test_string = "Hello from a syscall!";

Snake* snekkk;
void Snake_Render_Callback() {
	if(snekkk)
		snekkk->Render();
}

int sysinfo_win_x = 64;
int sysinfo_win_y = 64;
int ram_amount;

void SysInfo_Window_Render_Callback() {
	screen_fillrect(sysinfo_win_x, sysinfo_win_y + 24, 320, 200 - 24, 255, 255, 255);
	drawstring("Lemon OS Alpha", sysinfo_win_x, sysinfo_win_y + 24, 0, 0, 0);
	drawstring("CPU Vendor:", sysinfo_win_x, sysinfo_win_y + 44, 0, 0, 0, 1);
	drawstring(cpuid_info.vendor_string, sysinfo_win_x + 12 * 8, sysinfo_win_y + 44, 0, 0, 0, 1);
	drawstring("RAM:", sysinfo_win_x, sysinfo_win_y + 52, 0, 0, 0, 1);
	drawstring(itoa(ram_amount/1024), sysinfo_win_x + 5*8, sysinfo_win_y + 52, 0, 0, 0, 1);
	drawstring("MB", sysinfo_win_x + 15*8, sysinfo_win_y + 52, 0, 0, 0, 1);

	screen_fillrect(sysinfo_win_x + 10, sysinfo_win_y + 150, 100, 32, 128, 128, 128);
	drawstring("Crash!", sysinfo_win_x + 15, sysinfo_win_y + 150, 0, 0, 0, 2);
}

void SysInfo_Window_Mouse_Callback(int x, int y) {
	if(x > sysinfo_win_x + 10 && y > sysinfo_win_y + 150)
		fatal_error("You piece of shit, you did this to me", "ERR_SCREWED");
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
	ram_amount = mem_info.memory_high = mb_info.memoryHi;
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

	cpuid_info = cpuid_get_info();

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
	
	for (int i = 0; i < 30; i++) screen_update(); // Loading Bars are nice

	drawbitmap_noscale(video_mode.width / 2 - 32 * 2, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	keyboard_install();
	mouse_install();
	timer_install();

	for (int i = 0; i < 30; i++) screen_update(); // Loading Bars are nice

	drawbitmap_noscale(video_mode.width / 2 - 32 * 1, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	for (int i = 0; i < 30; i++) screen_update(); // Loading Bars are nice
	
	drawbitmap_noscale(video_mode.width / 2, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	WindowManager win_mgr(&video_mode); // Initialize Window Manager

	for (int i = 0; i < 30; i++) screen_update(); // Loading Bars are nice

	drawbitmap_noscale(video_mode.width / 2 + 32 * 1, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	Snake snake(10,34,500,400);
	snekkk = &snake;
	Window* snake_window = win_mgr.Window_New(10, 10, 500, 424, windowtype_framebuffer);
	snake_window->render_callback = Snake_Render_Callback;
	
	Window* sysinfo_window = win_mgr.Window_New(64, 64, 320, 200, windowtype_framebuffer);
	sysinfo_window->render_callback = SysInfo_Window_Render_Callback;
	sysinfo_window->mouse_callback = SysInfo_Window_Mouse_Callback;

	for (;;) {
		win_mgr.Update();
		snake.Relocate(snake_window->x, snake_window->y+24);
		snake.Update();
		sysinfo_win_x = sysinfo_window->x;
		sysinfo_win_y = sysinfo_window->y;
		screen_update();
	}
}