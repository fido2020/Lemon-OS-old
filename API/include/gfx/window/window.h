#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <core/types.h>
#include <core/syscall.h>

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint32_t flags;
	//surface_t*
	whandle_t* handle;
} win_info_t;

#define WM_WIN_NEW 1 // New Window
#define WM_WIN_DESTROY 2 // Destroy Window
#define WM_MOVE 3 // Move Window
#define WM_SETSIZE 4 // Set Window Size
#define WM_DRAW 5 // Redraw Window
#define WM_FORCE_QUIT 6 // Force quit application
#define WM_EXIT 7 // Exit Program
#define WM_SET_TITLE 8 // Set Window Title

handle_t create_window(win_info_t* wininfo);

#endif
