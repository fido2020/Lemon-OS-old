#pragma once

#include <stdint.h>
#include <video.h>

struct ConsoleCharacter {
	char c;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} __attribute__((packed));

class Console {
private:
	uint16_t width_characters;
	uint16_t height_characters;

	uint16_t x = 0;
	uint16_t y = 0;

	uint16_t startX = 0;
	uint16_t startY = 0;

	uint16_t width = 0;
	uint16_t height = 0;

	ConsoleCharacter* char_buffer;
public:
	Console(uint16_t x, uint16_t y, uint16_t _width, uint16_t _height);

	void putc(char c, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
	void puts(char* str, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

	void clear(uint8_t r, uint8_t g, uint8_t b);
	void refresh();

	void relocate(uint16_t x, uint16_t y);
};