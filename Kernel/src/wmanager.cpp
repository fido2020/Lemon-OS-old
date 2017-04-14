#include <graphics.h>

#include <wmanager.h>

using namespace Graphics;

vesa_mode_info_t* modeInfo;

Window::Window(int _x, int _y, int _width, int _height){
	//modeInfo = initGfx();

	x = _x;
	y = _y;

	width = _width;
	height = _height;
}

uint8_t randnum8() {

    static uint16_t seed = 0;
    return (uint8_t)(seed = (12657 * seed + 12345) % 256);
}

void Window::Paint(){

	fillrect(x,y,width,height,randnum8(),randnum8(),randnum8());
}

Desktop::Desktop(){
	modeInfo = initGfx();
	//List list = List();
	//children = list;
}

void Desktop::Paint(){

	//fillrect(0,0,200,200,33,99,255);
	fillrect(0,0,200,200,255,255,0);

	for(uint32_t i = 0; i < windowCount; i++)
        windows[i]->Paint();
}

Window* Desktop::CreateWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
	Window* win = new Window(x,y,width,height);

	if(windowCount == maxWindows);

	windows[windowCount] = win;

	windowCount++;

	return win;
}
