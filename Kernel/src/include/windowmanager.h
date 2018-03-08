#pragma once

#include <stdint.h>
#include <mischdr.h>
#include <list.h>
#include <window.h>

class WindowManager
{
public:
	WindowManager(video_mode_t* video_mode);
	void Update();
private:
	//List<GUIText> text;
	int mouse_x = 250;
	int mouse_y = 250;
	uint32_t screen_width;
	uint32_t screen_height;

	//List<Window*> windows = List<Window*>();

	void DrawMouseCursor();
};