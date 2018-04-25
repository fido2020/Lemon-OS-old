#pragma once

#include <stdint.h>
#include <mischdr.h>

typedef enum {
	Text,
	Graphical
} video_mode_type_t;


typedef struct
{
	unsigned int width;
	unsigned int height;
	unsigned char *data;
} bitmap_t;

void screen_putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void video_initialize(video_mode_t v);
void screen_update();
void screen_clear();
void screen_fillrect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b);
void drawchar(char c, int x, int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, int scale = 2);
void drawstring(char* str, int x, int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, int scale = 2);
void drawstring(char** str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale);
void screen_clear(uint8_t r, uint8_t g, uint8_t b);
void drawbitmap(int x, int y, int w, int h, uint8_t *data, int bmp_bpp);

void screen_fillrect_direct(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b);

void drawgrayscalebitmap(int x, int y, int w, int h, uint8_t* data);

video_mode_t get_video_mode();

void screen_clear_direct(uint8_t r, uint8_t g, uint8_t b);