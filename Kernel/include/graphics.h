#pragma once

#include <vesadrv.h>

#include <common.h>

typedef struct{
	int32_t x,y;
}Vector2;

namespace graphics{
	vesa_mode_info_t *initGfx();
	void putpixel(int x,int y, unsigned char r, unsigned char g, unsigned char b);
	void putpixel(int x,int y, uint32_t color);

	void fillrect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
	void fillrect(int x, int y, int w, int h, uint32_t colour);

	void drawstring(char* str);

	void UpdateScreen();

	class Bitmap{
		public:
			Bitmap();
		//private:

	};

	void drawchar(char character, int x, int y, uint8_t r, uint8_t g, uint8_t b);
}
