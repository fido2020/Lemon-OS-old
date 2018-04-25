#include <windowmanager.h>

#include <mouse.h>
#include <video.h>
#include <graphicsresources.h>
#include <stddef.h>
#include <memory.h>
#include <console.h>
#include <serial.h>
#include <string.h>
#include <math.h>

static int8_t* mouse_data;

WindowManager::WindowManager(video_mode_t* video_mode)
{
	screen_width = video_mode->width;
	screen_height = video_mode->height;

	screen_clear(96, 192, 192);

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

		if (mouse_x > (int)screen_width)
			mouse_x = screen_width;
		if (mouse_y > (int)screen_height)
			mouse_y = screen_height;
	}

	DrawMouseCursor();

	//screen_update();
}

void WindowManager::DrawMouseCursor() {
	drawgrayscalebitmap(mouse_x, mouse_y, 11, 18, mouse_cursor);
}

