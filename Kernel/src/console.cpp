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
	char_buffer = (uint8_t*)malloc(width_characters*height_characters * sizeof(ConsoleCharacter*));
	memset((uint8_t*)char_buffer, 0, width_characters*height_characters * sizeof(ConsoleCharacter*));
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

	char_buffer[y*width_characters*4 + x*4] = c;
	char_buffer[y*width_characters * 4 + x * 4 + 1] = r;
	char_buffer[y*width_characters * 4 + x * 4 + 2] = g;
	char_buffer[y*width_characters * 4 + x * 4 + 3] = b;
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
	memset((uint8_t*)char_buffer, 0, width_characters*height_characters * sizeof(ConsoleCharacter*));
}

void Console::refresh() {
	//screen_fillrect(startX, startY, width, height, 0, 0, 0);

	for (int i = 0; i < height_characters; i++) {
		for (int j = 0; j < width_characters; j++) {
			volatile ConsoleCharacter con_char = ((ConsoleCharacter*)char_buffer)[i * width_characters * 4 + j * 4];
			
			if (con_char.c == '\n') {
				i++;
			} else if (con_char.c != 0) {
				drawchar(con_char.c, startX + j * 8, startY + i * 8, 255, con_char.b, con_char.b, 1);
			}
		}
	}
}

void Console::relocate(uint16_t x, uint16_t y)
{
	startX = x;
	startY = y;
}