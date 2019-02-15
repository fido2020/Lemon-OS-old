#include <stdint.h>

#include <core/syscall.h>
#include <gfx/surface.h>
#include <gfx/graphics.h>
#include <gfx/window/window.h>

struct WindowRect{
	rect_t rect;
	surface_t* windows;
};

uint8_t buffer[1024*768*4+4]; // TODO: Get video mode syscall

handle_t window_handles[255];

void _start(){
	uint32_t desktop_handle;
	surface_t surface;
	surface.buffer = buffer;
	syscall(SYS_CREATE_DESKTOP, &desktop_handle, 0, 0, 0, 0);
	
	surface.width = 1024;
	surface.height = 768;
	surface.depth = 32;

	win_info_t winfo;
	winfo.width = 320;
	winfo.height = 320;
	winfo.x = 10;
	winfo.y = 105;
	winfo.flags = 0;

	uint32_t whandle;
	syscall(SYS_CREATE_WINDOW, &whandle, &winfo, 0, 0, 0);

	winfo.width = 640;
	winfo.height = 480;
	winfo.x = 100;
	winfo.y = 100;
	winfo.flags = 0;

	uint32_t whandle2;
	syscall(SYS_CREATE_WINDOW, &whandle2, &winfo, 0, 0, 0);

	for(;;){
		fillrect(0,0,1024,768,64,192,192, &surface);
		uint32_t amount;
		
		syscall(SYS_DESKTOP_GET_WIN_AMOUNT, desktop_handle, &amount, 0, 0, 0);
		syscall(SYS_DESKTOP_GET_WIN_HANDLES, desktop_handle, window_handles, 0, 0, 0);
		
		for(int i = 0; i < amount; i++){
			win_info_t wininfo;
			syscall(SYS_GET_WINDOW_INFO, window_handles[i], &wininfo,0, 0, 0);
			fillrect(wininfo.x,wininfo.y,wininfo.width,24,64,64,64,&surface);
			vector2i_t pos = {wininfo.x, wininfo.y + 24};
			rect_t rect = {{0, 0},{wininfo.width, wininfo.height}};
			syscall(20,&surface, window_handles[i], &rect, &pos, 0);
		}
		syscall(SYS_PAINT_DESKTOP,desktop_handle,&surface, 0, 0, 0);
	}
}
