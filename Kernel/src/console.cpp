#include <console.h>

namespace console {

	uint16_t width_characters;
	uint16_t height_characters;

	uint16_t x = 0;
	uint16_t y = 0;

	video_mode_t video_mode;

	void initialize(video_mode_t v) {
		video_mode = v;
		width_characters = (v.width - v.width % 8) / 8;
		height_characters = (v.height - v.height % 8) / 8;
	}

	void putc(char c, uint8_t r, uint8_t g, uint8_t b) {
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
			drawchar(c, x * 8, y * 8, r, g, b, 1);
			x++;
			break;
		 }
	}

	void puts(char* str, uint8_t r, uint8_t g, uint8_t b) {
		int i = 0;
		while (str[i])
			putc(str[i++], r, g, b);
	}

	void clear(uint8_t r, uint8_t g, uint8_t b) {
		screen_clear(r, g, b);
	}
}