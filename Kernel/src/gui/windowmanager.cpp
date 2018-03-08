#include <windowmanager.h>

#include <mouse.h>
#include <video.h>
#include <graphicsresources.h>
#include <stddef.h>
#include <window.h>
#include <memory.h>
#include <console.h>
#include <serial.h>
#include <string.h>

static int8_t* mouse_data;

Window* win;
Window* win2;
Window* win3;

WindowManager::WindowManager(video_mode_t* video_mode)
{
	screen_width = video_mode->width;
	screen_height = video_mode->height;

	screen_clear(96, 192, 192);

	win = Window_new(10, 20, 320, 200);
	win2 = Window_new(150, 20, 200, 300);
	win3 = Window_new(100, 100, 200, 150);

	

	//windows.add_back(win);

	Window_draw(win);

	Window_draw(win2);
	Window_draw(win3);

	screen_update();

	mouse_install();
}

void WindowManager::Update()
{
	screen_clear(96, 192, 192);

	if (mouse_data_updated()) {
		mouse_data = mouse_get_data();

		mouse_x += mouse_data[1];
		mouse_y -= mouse_data[2];

		if (mouse_x < 0)
			mouse_x = 0;
		if (mouse_y < 0)
			mouse_y = 0;

		if (mouse_x > screen_width)
			mouse_x = screen_width;
		if (mouse_y > screen_height)
			mouse_y = screen_height;
	}

	DrawMouseCursor();

	//screen_update();
}

void WindowManager::DrawMouseCursor() {
	drawgrayscalebitmap(mouse_x, mouse_y, 11, 18, mouse_cursor);
}

