#include <console.h>

#include <memory.h>
#include <serial.h>
#include <string.h>

Console::Console(uint16_t x, uint16_t y, uint16_t _width, uint16_t _height) {
	startX = x;
	startY = y;
	width = _width;
	height = _height;
	width_characters = (width - width % 8) / 8;
	height_characters = (height - height % 8) / 8;
	this->x = 0;
	this->y = 0;
	char_buffer = (char*)malloc(width_characters*height_characters);
	memset((uint8_t*)char_buffer, 'a', width_characters*height_characters);
}

void Console::putc(char c, uint8_t r, uint8_t g, uint8_t b) {
	switch (c) {
	case '\n':
		x = 0;
		y++;
		break;
	case '\t':
		x += 4;
		break;
	case '\b':
		x--;
		break;
	default:
		drawchar(c, startX + x * 8, startY + y * 8, r, g, b, 1);
		x++;
		break;
	 }

	char_buffer[y*width_characters + x] = c;

	if (x >= width_characters) {
		y++;
		x = 0;
	}
}

void Console::puts(char* str, uint8_t r, uint8_t g, uint8_t b) {
	int i = 0;
	while (str[i])
		putc(str[i++], r, g, b);
}

void Console::clear(uint8_t r, uint8_t g, uint8_t b) {
	screen_fillrect(startX, startY, width, height, r, g, b);
	x = 0;
	y = 0;
	memset((uint8_t*)char_buffer, 0, width_characters*height_characters * sizeof(ConsoleCharacter));
}

void Console::refresh() {
	//screen_fillrect(startX, startY, width, height, 0, 0, 0);

	for (int i = 0; i < height_characters; i++) {
		for (int j = 0; j < width_characters; j++) {
			char con_char = char_buffer[i * width_characters + j];
			
			if (con_char == '\n') {
				i++;
			} else if (con_char != 0) {
				write_serial(con_char);
				drawchar(con_char, startX + j * 8, startY + i * 8, 255, 255, 255, 1);
			}
		}
	}
}

void Console::relocate(uint16_t x, uint16_t y)
{
	startX = x;
	startY = y;
}