#pragma once

#include <video.h>

class Widget {
public:
	virtual void Render();
protected:
	int x;
	int y;
	int win_x;
	int win_y;
}; 


class Widget_Text : public Widget {
public:
	Widget_Text(int win_x, int win_y, int x, int y, char* text = nullptr, rgb_colour_t colour = { 0, 0, 0 }, int size = 1);
	void Text_Set(char* text);
	void Render();
protected:
	char* text;
	rgb_colour_t colour = { 0, 0, 0 };
	int size = 1;
};