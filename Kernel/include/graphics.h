#include <vesadrv.h>

namespace graphics{
	vesa_mode_info_t *initGfx();
	void putpixel(int x,int y, unsigned char r, unsigned char g, unsigned char b);
	void putpixel(int x,int y, uint32_t color);
	
	void fillrect(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
	void fillrect(int x, int y, int w, int h, uint32_t colour);
	
	void drawchar(char c);
	void drawstring(char* str);
	
	class Bitmap{
		public:
			Bitmap();
		//private:
			
	};
}