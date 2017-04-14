#pragma once

#include <vesadrv.h>

#include <common.h>

typedef struct{
	int32_t x,y;
}Vector2;

namespace Graphics{
	vesa_mode_info_t *initGfx();
	void putpixel(int x,int y, unsigned char r, unsigned char g, unsigned char b);
	void putpixel(int x,int y, uint32_t color);

	void fillrect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
	void fillrect(int x, int y, int w, int h, uint32_t colour);

	void updateScreen();

	class Bitmap{
		public:
			Bitmap();
		//private:

	};

	struct RGBColour{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	void drawchar(char character, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale = 2);
	void drawstring(char* str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale = 2);

	void drawglyph(char glyph, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale = 2);
}
