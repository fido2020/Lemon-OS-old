#include <widget.h>

#include <string.h>
#include <memory.h>

Widget_Text::Widget_Text(int win_x, int win_y, int x, int y, char * text, rgb_colour_t colour, int size)
{
	this->x = x;
	this->y = y;
	this->size = size;
	if (text != nullptr) {
		this->text = (char*)malloc(strlen(text)+1);
		strcpy(this->text, text);
	}

	this->colour = colour;
	this->size = size;
	this->win_x = win_x;
	this->win_y = win_y;
}

void Widget_Text::Text_Set(char * text)
{
	if (text != nullptr) {
		this->text = (char*)malloc(strlen(text) + 1);
		strcpy(this->text, text);
	}
}

void Widget_Text::Render()
{
	drawstring(text, win_x + x, win_y + y,colour.r,colour.g,colour.b,size);
}