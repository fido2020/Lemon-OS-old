#include <window.h>

#include <video.h>
#include <memory.h>
#include <stdint.h>
#include <math.h>

uint8_t r;
uint8_t g;
uint8_t b;

Window* Window_new(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	Window* win = (Window*)malloc(sizeof(Window));
	win->x = x;
	win->y = y;
	win->width = width;
	win->height = height;
	return win;
}

void Window_draw(Window* win) {
	r = rand();
	g = rand();
	b = rand();

	screen_fillrect(win->x, win->y, win->width, win->height, r, g, b);
}