#include <GUI.h>

#include <mouse.h>
#include <video.h>
#include <graphicsresources.h>
#include <stddef.h>

static int8_t* mouse_data;

GUI::GUI(video_mode_t* video_mode)
{
	screen_width = video_mode->width;
	screen_height = video_mode->height;

	mouse_install();

	screen_clear(96, 192, 192);
	//drawstring("Lemon OS", 10, 10, 255, 255, 255, 4);
	//text = List<GUIText>();
	//GUIText t;
	//t.text = "Lemon OS";
	//t.scale = 4;
	//t.type = Text;

	//text.add_back(t);
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

	screen_update();
}

void GUI::DrawMouseCursor() {
	drawgrayscalebitmap(mouse_x, mouse_y, 11, 18, mouse_cursor);
}

