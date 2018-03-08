#include <console.h>

#include <memory.h>
#include <serial.h>
#include <string.h>

Console::Console(video_mode_t v, uint16_t x, uint16_t y, uint16_t _width, uint16_t _height) {
	video_mode = v;
	startX = x;
	startY = y;
	width = _width;
	height = _height;
	width_characters = (width - width % 8) / 8;
	height_characters = (height - height % 8) / 8;

	char_buffer = (ConsoleCharacter*)malloc(width_characters * height_characters * sizeof(ConsoleCharacter));

	ConsoleCharacter con_char;
	con_char.c = ' ';
	con_char.r = 0;
	con_char.b = 0;
	con_char.g = 0;

	for (int i = 0; i < width_characters * height_characters; i++) {
		char_buffer[i] = con_char;
	}
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

	if (x >= width_characters) {
		y++;
		x = 0;
	}

	ConsoleCharacter con_char;
	con_char.c = c;
	con_char.r = r;
	con_char.g = g;
	con_char.b = b;
	char_buffer[width_characters*y + x] = con_char;
}

void Console::puts(char* str, uint8_t r, uint8_t g, uint8_t b) {
	int i = 0;
	while (str[i])
		putc(str[i++], r, g, b);
}

void Console::clear(uint8_t r, uint8_t g, uint8_t b) {
	screen_fillrect(startX, startY, width, height, r, g, b);
}

void Console::refresh() {
	clear(0, 0, 0);
	x = 0;
	y = 0;

	for (int i = 0; i < width_characters * height_characters; i++) {
		ConsoleCharacter con_char = char_buffer[i];
		putc(con_char.c, con_char.r, con_char.g, con_char.b);
		write_serial(con_char.c);
	}
}