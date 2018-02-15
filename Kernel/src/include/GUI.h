#pragma once

#include <stdint.h>
#include <mischdr.h>
#include <list.h>

class GUIObject {
public:
	int x;
	int y;
	int type;
};

class GUIText : public GUIObject {
public:
	char* text;
	int scale;
};

enum GUIObjectType {
	text
};



class GUI
{
public:
	GUI(video_mode_t* video_mode);
	void Update();
private:
	//List<GUIText> text;
	int mouse_x = 250;
	int mouse_y = 250;
	uint32_t screen_width;
	uint32_t screen_height;

	void DrawMouseCursor();
};