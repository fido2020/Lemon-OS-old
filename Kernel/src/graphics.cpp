#include <vesadrv.h>
#include <common.h>
#include <conio.h>

#include <graphics.h>

#include <font.h>

namespace Graphics{

	vesa_mode_info_t *modeInfo;
	uint32_t vram = 0;

	unsigned char* dblBuffer;
	unsigned char* screen;
	bool initialized = false;

	vesa_mode_info_t * initGfx(){
		if(!initialized){
			modeInfo = enterGraphicsMode();
			vram = modeInfo->physbase;
			//if(vram == 0)
			//	panic("ERR_GFX_MODE_FAILED", "Error whilst setting graphics mode.",false);

			screen = (unsigned char*)vram;
			dblBuffer = (unsigned char*)malloc((modeInfo->height * (modeInfo->width * (modeInfo->bpp/8))));

			fillrect(0,0,modeInfo->width,modeInfo->height,0,0,0);

			//memset(dblBuffer,80,modeInfo->width*modeInfo->height*(modeInfo->bpp/8));

			//updateScreen();

			//InitFonts();

			initialized = true;
		}

		return modeInfo;
	}

	void putpixel(int x,int y, uint8_t r, uint8_t g, uint8_t b) {
		putpixel(x,y,(r << 16) + (g << 8) + b);
	}

	/**/void putpixel(int x,int y, uint32_t colour) {
		unsigned where = y /** modeInfo->pitch*/ + (x * (modeInfo->bpp/8));
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

	/** /void putpixel(int x,int y, uint32_t colour) {
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

	void drawchar(char c, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale){
		for(int i = 0; i < 8; i++)
        {
            int row = font_default[c][i];
            for(int j = 0; j < 8; j++)
            {
                if((row & ( 1 << j )) >> j)
                    fillrect(x+j*scale,y+i*scale,scale,scale,r,g,b);
            }
        }
	}

	void drawglyph(char glyph, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale){
		for(int i = 0; i < 8; i++)
        {
            int row = glyph_default[glyph][i];
            for(int j = 0; j < 8; j++)
            {
                if((row & ( 1 << j )) >> j)
                    fillrect(x+j*scale,y+i*scale,scale,scale,r,g,b);
            }
        }
	}

	void drawstring(char* str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale){
		int xOffset = 0;
		while(*str != 0){
			drawchar(*str,x + xOffset,y,r,g,b,scale);
			xOffset += scale*8;
			str++;
		}
	}

	void updateScreen(){
      unsigned int visibleBytesPerLine = modeInfo->width * (modeInfo->bpp / 8);
      uint8_t *dest = screen;
      uint8_t *src = dblBuffer;

      for(int y = 0; y < modeInfo->height; y++) {
         memcpy(dest, src, visibleBytesPerLine);
         dest += modeInfo->pitch;
         src += visibleBytesPerLine;
      }
   }
}
