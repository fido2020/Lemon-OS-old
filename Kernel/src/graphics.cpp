#include <vesadrv.h>
#include <common.h>
#include <vesadrv.h>

#include <graphics.h>

namespace graphics{
	vesa_mode_info_t *modeInfo;
	uint32_t vram = 0;
	unsigned char* screen;
	bool initialized = false;
	
	vesa_mode_info_t * initGfx(){
		if(!initialized){
			modeInfo = EnterGraphicsMode();
			vram = modeInfo->physbase;
			//if(vram == 0)
			//	panic("ERR_GFX_MODE_FAILED", "Error whilst setting graphics mode.",true);
			
			screen = (unsigned char*)vram;
			
			initialized = true;
		}
		return modeInfo;
	}
	
	void putpixel(int x,int y, uint8_t r, uint8_t g, uint8_t b) {
		putpixel(x,y,(r << 16) + (g << 8) + b);
	}
	
	void putpixel(int x,int y, uint32_t colour) {
		unsigned where = y * modeInfo->pitch + (x * (modeInfo->bpp/8));
		screen[where] = colour & 255;              // BLUE
		screen[where + 1] = (colour >> 8) & 255;   // GREEN
		screen[where + 2] = (colour >> 16) & 255;  // RED
	}
	
	void fillrect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b){
		fillrect(x,y,w,h,(r << 16) + (g << 8) + b);
	}
	
	void fillrect(int x, int y, int w, int h, uint32_t colour) {
		int bpp = modeInfo->bpp; 
		int pitch = modeInfo->pitch;
		uint32_t where;
		for(int i=0;i<h;i++){
			for(int j=0;j<w;j++){
				where = (y+i) * pitch + ((x+j) * (bpp/8));
				screen[where] = colour & 255;              // BLUE
				screen[where + 1] = (colour >> 8) & 255;   // GREEN
				screen[where + 2] = (colour >> 16) & 255; // RED
			}
		}
	}
}