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

#include <hal.h>

extern uint32_t physalloc_used_blocks;
cpuid_info_t cpuid_info;
int ram_amount;

WindowManager* wm;
Console* sh_con;

void Shell_render_callback(int x, int y){
	sh_con->relocate(x, y + 24);
	sh_con->refresh();
}

void Shell_process(){
sh_restart:
	Window* win = wm_getinstance()->Window_New(10,10,500,375, windowtype_framebuffer);
	Console con = Console(0,0,500,375-24);
	Shell sh = Shell(&con);
	sh_con = &con;
	win->render_callback = Shell_render_callback;
	while(win->exists){
		//con.relocate(sh_win->x,sh_win->y);
		while(win->event_stack->num){
			if((*win->event_stack)[0]->type == event_key)
				sh.Update(true, ((KeyEventData*)(*win->event_stack)[0]->data)->key/*((KeyEvent*)((*win->event_stack)[0]->dervied))->key*/);
			(*win->event_stack)[0]->~Event();
			free((*win->event_stack)[0]);
			win->event_stack->remove_at(0);
		}
	}
	free(win);
	goto sh_restart;
}

Snake* snekkk;

void Snake_render_callback(int x, int y){
	snekkk->Relocate(x,y + 24);
	snekkk->Render();
}

void Snake_process(){
	Window* win = wm_getinstance()->Window_New(64,64,640,480,windowtype_framebuffer);
	Snake snake = Snake(64,64,640,480 - 24);
	snekkk = &snake;
	win->render_callback = Snake_render_callback;
	while(win->exists){
		while(win->event_stack->num){
			if(win->event_stack->operator[](0)->type == event_key){
				snake.OnKeyPressed(((KeyEventData*)(*win->event_stack)[0]->data)->key);
			}
			win->event_stack->operator[](0)->~Event(); // Run event class destructor
			free(win->event_stack->operator[](0)); // Unallocate event class
			win->event_stack->remove_at(0);
		}
		snake.Update();
	}
	free(win);
	for(;;);
}

void WindowManager_process(){
	WindowManager winmgr = WindowManager(&HAL::video_mode);
	wm = & winmgr;
	create_process((void*)Shell_process);
	//create_process((void*)Snake_process);
	for(;;){
		screen_clear(96, 192, 192);
		winmgr.Update();
		screen_update();
	}
}

void idle2(){
	for(;;);
}

extern "C"
void kmain(uint32_t mb_info_addr) {
	multiboot_info_t mb_info = *((multiboot_info_t*)mb_info_addr);

	HAL::initialize_all(mb_info);

	write_serial_string("xcv");

	multiboot_module_t initrd_module = *((multiboot_module_t*)mb_info.modsAddr);
	cpuid_info = cpuid_get_info();

	memory_info_t mem_info = HAL::mem_info;
	video_mode_t video_mode = HAL::video_mode;

	if (mb_info.modsCount < 1) {
		fatal_error("The initrd was not passed in boot config", "NO_INITRD"); // Send an error message if the initrd was not passed in boot config
	}
	else if (mem_info.memory_high + mem_info.memory_low < 32000) fatal_error("Not enough memory! (< 32MB)", "ERR_NOT_ENOUGH_MEM"); // Throw a fatal if there isnt enough memory at the moment 24MB is demanded as the minimum
	else if (!(cpuid_info.features_edx & CPUID_EDX_SSE2)) fatal_error("CPU does not support SSE2", "ERR_NO_SSE2");

	initrd_init(initrd_module.mod_start,initrd_module.mod_end - initrd_module.mod_start);

	if(strcmp((char*)mb_info.cmdline,"kshell") == 0){
		Console kshell_console = Console(0,0,video_mode.width, video_mode.height);
		kshell_console.puts("Lemon Kernel Shell, type 'cmds' to list all shell commands");
		Shell sh(&kshell_console);
		keyboard_install();
		for(;;){
			sh.Update(key_updated(),get_key());
			kshell_console.refresh();
			screen_update();
		}
		return;
	}

	initrd_init(initrd_module.mod_start,initrd_module.mod_end - initrd_module.mod_start);
	write_serial_string("\r\n0x");
	write_serial_string(itoa(kernel_pages_allocate(1), 0, 16));
	write_serial_string("\r\n0x");
	write_serial_string(itoa(kernel_pages_allocate(2), 0, 16));
	write_serial_string("\r\n0x");
	write_serial_string(itoa(kernel_pages_allocate(20), 0, 16));
	write_serial_string("\r\n0x");
	write_serial_string(itoa(kernel_pages_allocate(20), 0, 16));

	
	void* splashscreen_bmp = (void*)(initrd_read(2));
	void* progress_bmp = (void*)(initrd_read(0));

	screen_clear(0, 0, 0);

	drawbitmap_noscale(video_mode.width / 2 - 260, video_mode.height / 2 - 250, 525, 366, (uint8_t*)(splashscreen_bmp + 54), 24);
	screen_update();
	

	drawbitmap_noscale(video_mode.width / 2 - 32 * 2, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	keyboard_install();
	mouse_install();
	multitasking_init();


	drawbitmap_noscale(video_mode.width / 2 - 32 * 1, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	drawbitmap_noscale(video_mode.width / 2, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	drawbitmap_noscale(video_mode.width / 2 + 32 * 1, video_mode.height / 2 + 250, 32, 32, (uint8_t*)(progress_bmp + 54), 24);
	screen_update();

	screen_clear(96, 192, 192);
	screen_update();

	create_process((void*)idle2);
	create_process((void*)WindowManager_process);
	
	timer_install(1000); // The scheduler will start running when the timer is set up.
	
	for(;;);
}