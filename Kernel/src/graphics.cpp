#include <vesadrv.h>
#include <conio.h>

namespace Graphics{
	vesa_mode_info_t *modeInfo;
	uint32_t vram = 0;
	unsigned char* screen;
	
	void init(){
		modeInfo = EnterGraphicsMode();
		vram = (unsigned char*)modeInfo->physbase;
		if(vram == 0)
			panic("ERR_GFX_MODE_FAILED", "Error whilst setting graphics mode.",true);
	}
	
	static void putpixel(int x,int y, unsigned char r, unsigned char g, unsigned char b) {
		putpixel(x,y,(r << 16) + (g << 8) + b);
	}
	
	static void putpixel(int x,int y, int color) {
		unsigned where = x*4 + y*3200;
		screen[where] = color & 255;              // BLUE
		screen[where + 1] = (color >> 8) & 255;   // GREEN
		screen[where + 2] = (color >> 16) & 255;  // RED
	}
	
	static void fillrect(unsigned char* loc,unsigned char w, unsigned char h,unsigned char r, unsigned char g, unsigned char b) {
		unsigned char *where = screen + loc;
		int i, j;
	 
		for (i = 0; i < w; i++) {
			for (j = 0; j < h; j++) {
				//putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
				where[j*4] = r;
				where[j*4 + 1] = g;
				where[j*4 + 2] = b;
			}
			where+=3200;
		}
	}
}