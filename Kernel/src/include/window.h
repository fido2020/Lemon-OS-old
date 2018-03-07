#pragma once

#include <stdint.h>

struct Window {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
};

Window* window_create(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void window_draw(Window* win);