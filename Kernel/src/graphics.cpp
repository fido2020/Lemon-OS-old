#include <vesadrv.h>
#include <common.h>
#include <conio.h>

#include <graphics.h>

#include <font.h>

namespace graphics{

	vesa_mode_info_t *modeInfo;
	uint32_t vram = 0;

	unsigned char* dblBuffer;
	unsigned char* screen;
	bool initialized = false;

	vesa_mode_info_t * initGfx(){
		if(!initialized){
			modeInfo = EnterGraphicsMode();
			vram = modeInfo->physbase;
			//if(vram == 0)
			//	panic("ERR_GFX_MODE_FAILED", "Error whilst setting graphics mode.",false);

			screen = (unsigned char*)vram;
			dblBuffer = (unsigned char*)malloc(modeInfo->height * modeInfo->pitch + modeInfo->width * (modeInfo->bpp/8));

			fillrect(0,0,modeInfo->width,modeInfo->height,80,80,80);

			memset(dblBuffer,80,sizeof(screen));

			UpdateScreen();

			//InitFonts();

			initialized = true;
		}

		return modeInfo;
	}

	void putpixel(int x,int y, uint8_t r, uint8_t g, uint8_t b) {
		putpixel(x,y,(r << 16) + (g << 8) + b);
	}

	/**/void putpixel(int x,int y, uint32_t colour) {
		unsigned where = y * modeInfo->pitch + (x * (modeInfo->bpp/8));
		dblBuffer[where] = colour & 255;              // BLUE
		dblBuffer[where + 1] = (colour >> 8) & 255;   // GREEN
		dblBuffer[where + 2] = (colour >> 16) & 255;  // RED
	}

	void fillrect(int x, int y, int w, int h, uint32_t colour) {
		int bpp = modeInfo->bpp;
		int pitch = modeInfo->pitch;
		uint32_t where = 0;
		for(int i=0;i<h;i++){
			for(int j=0;j<w;j++){
				where = (y+i) * pitch + ((x+j) * (bpp/8));
				dblBuffer[where] = colour & 255;             // BLUE
				dblBuffer[where + 1] = (colour >> 8) & 255;  // GREEN
				dblBuffer[where + 2] = (colour >> 16) & 255; // RED
			}
		}
	}/**/

	/*void putpixel(int x,int y, uint32_t colour) {
		unsigned where = y * modeInfo->pitch + (x * (modeInfo->bpp/8));
		screen[where] = colour & 255;              // BLUE
		screen[where + 1] = (colour >> 8) & 255;   // GREEN
		screen[where + 2] = (colour >> 16) & 255;  // RED
	}

	void fillrect(int x, int y, int w, int h, uint32_t colour) {
		int bpp = modeInfo->bpp;
		int pitch = modeInfo->pitch;
		uint32_t where = 0;
		for(int i=0;i<h;i++){
			for(int j=0;j<w;j++){
				where = (y+i) * pitch + ((x+j) * (bpp/8));
				screen[where] = colour & 255;             // BLUE
				screen[where + 1] = (colour >> 8) & 255;  // GREEN
				screen[where + 2] = (colour >> 16) & 255; // RED
			}
		}
	}/**/

	void fillrect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b){
		fillrect(x,y,w,h,(r << 16) + (g << 8) + b);
	}

	void drawchar(char character, int x, int y, uint8_t r, uint8_t g, uint8_t b){
		int* fontchar = font_default[character];
		for(int i=0;i<12;i++){
			for(int j=0;j<8;j++){
				if(fontchar[i*j] == 1){
					putpixel(x+j,y+i,r,g,b);
				}
			}
		}
	}

	void UpdateScreen(){
		int bpp = modeInfo->bpp;
		int pitch = modeInfo->pitch;
		uint32_t where = 0;
		for(int i=0;i<modeInfo->height;i++){
			for(int j=0;j<modeInfo->width;j++){
				where = i * pitch + (j * (bpp/8));
				screen[where] = dblBuffer[where] & 255;        // BLUE
				screen[where + 1] = (dblBuffer[where+1] >> 8) & 255;  // GREEN
				screen[where + 2] = (dblBuffer[where+2] >> 16) & 255;
			}
		}
	}
}
