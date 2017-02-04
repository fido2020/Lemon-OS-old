#include <list.h>

class Window{
	public:
		Window(int x, int y, int width, int height);
		void Paint();
	private:
			int x,y,width,height;
};

class Desktop{
	public:
		Desktop();
		Window* CreateWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void Paint();
	private:
		Window* windows[128];
		uint32_t windowCount = 0;
		uint32_t maxWindows = 128;
};