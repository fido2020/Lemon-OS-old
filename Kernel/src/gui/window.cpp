#include <window.h>

#include <video.h>
#include <memory.h>
#include <stdint.h>
#include <math.h>

uint8_t r;
uint8_t g;
uint8_t b;

<<<<<<< HEAD
=======
/*Window::Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Window::draw() {
	r = random();
	g = random();
	b = random();

	screen_fillrect(x, y, width, height, r, g, b);
}*/

>>>>>>> 17e9ca9a679e395e7e3bc93ec5eb2a2a0cd4790c
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