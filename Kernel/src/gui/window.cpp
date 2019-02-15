#include <window.h>

#include <video.h>
#include <memory.h>

#include <serial.h>
#include <hal.h>

desktop_t* main_desktop = 0;

desktop_t* get_main_desktop(){
	return main_desktop;
}

void set_main_desktop(desktop_t* desktop){
	main_desktop = desktop;
}