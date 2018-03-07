#include <window.h>

#include <video.h>
#include <memory.h>
#include <serial.h>
#include <string.h>

uint8_t r;
uint8_t g;
uint8_t b;

static uint8_t random() {

	uint16_t seed = 0;
	return (uint8_t)(seed = (12657 * seed + 12345) % 256);
}

Window* window_create(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	Window* win = (Window*)malloc(sizeof(Window));

	win->x = x;
	win->y = y;
	win->width = width;
	win->height = height;

	return win;
}

void window_draw(Window* win) {
	r = random();
	g = random();
	b = random();

	write_serial_string(itoa(win->width));
	screen_fillrect(win->x, win->y, win->width, win->height, r, g, b);
}