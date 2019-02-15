#include <stdint.h>

#include <core/types.h>
#include <gfx/window/window.h>

handle_t create_window(win_info_t* wininfo){
	handle_t h;
	syscall(SYS_CREATE_WINDOW,&h,wininfo,0,0,0);
	return h;
}
