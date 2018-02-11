#pragma once

#include <stdint.h>
#include <mischdr.h>

struct GUIObject {
	int x;
	int y;
};

struct GUIText : GUIObject {
	char* text;
	int scale;
};

struct GUINode
{
	GUINode* next;
	GUINode* prev;
	GUIObject obj;
	int index;
};

class GUINodeList {
public:
	void add_back(GUIObject obj);
	void add_front(GUIObject obj);
	void remove_back();
	void remove_front();
	GUIObject get_front();
	GUIObject get_back();
	GUIObject get_at_index(int index);
	int get_length();
	GUIObject  operator[] (unsigned pos);
private:
	GUINode* front;
	GUINode* back;
	int num;
};

class GUI
{
public:
	GUI(video_mode_t* video_mode);
	void Update();
private:
	GUINodeList objs;
	int mouse_x = 250;
	int mouse_y = 250;
	uint32_t screen_width;
	uint32_t screen_height;

	void DrawMouseCursor();
};

