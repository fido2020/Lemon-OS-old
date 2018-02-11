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
	GUIText text;
	text.text = "Lemon OS";
	text.scale = 4;

	objs.add_back(text);
}

void GUI::Update()
{
	if (mouse_data_updated()) {
		//screen_clear(96, 192, 192);

		screen_fillrect(mouse_x, mouse_y, 11, 18, 96, 192, 192);

		drawstring("Lemon OS", 10, 10, 255, 255, 255, 4);

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
}

void GUI::DrawMouseCursor() {
	drawgrayscalebitmap(mouse_x, mouse_y, 11, 18, mouse_cursor);
}

void GUINodeList::add_back(GUIObject obj) {
	GUINode node;
	node.obj = obj;
	node.index = num;
	node.prev = back;
	node.next = nullptr;
	back->next = &node;
	back = &node;
	num++;
}

GUIObject GUINodeList::get_at_index(int index) {
	GUINode* node = front;
	while (node->index != index && node->next != nullptr) {
		node = node->next;
	}
	return node->obj;
}

GUIObject GUINodeList::operator[](unsigned pos) {
	return get_at_index(pos);
}