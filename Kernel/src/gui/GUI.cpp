#include <GUI.h>

#include <mouse.h>
#include <video.h>
#include <graphicsresources.h>
#include <stddef.h>
#include <window.h>
#include <memory.h>
#include <console.h>

static int8_t* mouse_data;

Console console(get_video_mode(), 20, 20, 320, 200);

GUI::GUI(video_mode_t* video_mode)
{
	screen_width = video_mode->width;
	screen_height = video_mode->height;

	screen_clear(96, 192, 192);

	console.clear(0, 0, 0);

	console.puts("Lemon v0.1\n",255,255,255);
	console.puts("x",255,255,255);

	screen_update();

	mouse_install();
}

void GUI::Update()
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

void GUI::DrawMouseCursor() {
	drawgrayscalebitmap(mouse_x, mouse_y, 11, 18, mouse_cursor);
}

