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
	char_buffer = (ConsoleCharacter*)malloc(width_characters*height_characters*sizeof(ConsoleCharacter));
	memset(char_buffer, 0, width_characters*height_characters);
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
		char_buffer[y*width_characters + x].c = 0;
		x--;
		break;
	default:
		drawchar(c, startX + x * 8, startY + y * 8, r, g, b, 1);
		x++;
		break;
	 }

	char_buffer[y*width_characters + x] = {c, r, g, b};

	if (x >= width_characters) {
		y++;
		x = 0;
	}

	if(y >= height_characters){
		for(int i = 1; i < height_characters; i++){
			memcpy((void*)((uint32_t)char_buffer + (i-1)*width_characters*sizeof(ConsoleCharacter)),(void*)((uint32_t)char_buffer + i*width_characters*sizeof(ConsoleCharacter)),width_characters*sizeof(ConsoleCharacter));
			memset((void*)((uint32_t)char_buffer + i*width_characters*sizeof(ConsoleCharacter)), 0, width_characters*sizeof(ConsoleCharacter));
		}
		y--;
	}
	
}

void Console::puts(char* str, uint8_t r, uint8_t g, uint8_t b) {
	int i = 0;
	while (str[i])
		putc(str[i++], r, g, b);
}

void Console::clear(uint8_t r, uint8_t g, uint8_t b) {
	//screen_fillrect(startX, startY, width, height, r, g, b);
	x = 0;
	y = 0;
	memset((uint8_t*)char_buffer, 0, width_characters*height_characters * sizeof(ConsoleCharacter));
}

void Console::refresh() {
	screen_fillrect(startX, startY, width, height, 0, 0, 0);

	for (int i = 0; i < height_characters; i++) {
		for (int j = 0; j < width_characters; j++) {
			ConsoleCharacter con_char = char_buffer[i * width_characters + j];
			
			if (con_char.c == '\n') {
				//i++;
			} else if (con_char.c != 0) {
				drawchar(con_char.c, startX + j * 8, startY + i * 8, con_char.r, con_char.g, con_char.b, 1);
			}
		}
	}
}

void Console::relocate(uint16_t x, uint16_t y)
{
	startX = x;
	startY = y;
}