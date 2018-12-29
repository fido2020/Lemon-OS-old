#include <window.h>

#include <video.h>
#include <memory.h>

Window::Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t type) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->type = type;

	/*if (type == windowtype_gui) {
		widgets = (List<Widget*>*)malloc(sizeof(List<Widget>));
		//*widgets = List<Widget*>();
	}*/

	if (type == windowtype_console) {
		console = (Console*)malloc(sizeof(Console));
		*console = Console(x + 32, y, width, height - 32);
	}
	r = WINDOW_DEFAULT_BG_COLOUR_R;
	g = WINDOW_DEFAULT_BG_COLOUR_G;
	b = WINDOW_DEFAULT_BG_COLOUR_B;
}
Window::Window() {
	r = WINDOW_DEFAULT_BG_COLOUR_R;
	g = WINDOW_DEFAULT_BG_COLOUR_G;
	b = WINDOW_DEFAULT_BG_COLOUR_B;
	type = windowtype_gui;
	x = 0;
	y = 0;
	width = 320;
	height = 200;
}

void Window::Render() {
	if(active && !(flags & WINDOW_FLAGS_NODECORATION))
		screen_fillrect(x, y, width, 24, WINDOW_TITLEBAR_ACTIVE_COLOUR_R, WINDOW_TITLEBAR_ACTIVE_COLOUR_G, WINDOW_TITLEBAR_ACTIVE_COLOUR_B);
	else if(!(flags & WINDOW_FLAGS_NODECORATION)) screen_fillrect(x, y, width, 24, WINDOW_TITLEBAR_COLOUR_R, WINDOW_TITLEBAR_COLOUR_G, WINDOW_TITLEBAR_COLOUR_B);

	screen_fillrect(x + width - 20, y + 4, 16, 16, 255, 0, 0);
	drawchar('X', x + width - 16, y + 8, 255, 255, 255, 1);

	if (type == windowtype_gui && !(flags & WINDOW_FLAGS_NODECORATION)) {
		screen_fillrect(x, y + 24, width, height - 24, r, g, b);
	}
	else if(type == windowtype_console) console->refresh();
	screen_fillrect(x, y, 1, height, 0, 0, 0);
	screen_fillrect(x + width - 1, y, 1, height, 0, 0, 0);
	screen_fillrect(x, y, width, 1, 0, 0, 0);
	screen_fillrect(x, y + height - 1, width, 1, 0, 0, 0);
}