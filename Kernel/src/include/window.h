#pragma once

#include <stdint.h>

/*class Window {
public:
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;

public:
	Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void draw();

};*/

struct Window {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
};

Window* Window_new(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void Window_draw(Window* win);