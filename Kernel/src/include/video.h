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

void screen_putpixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b);
void video_initialize(video_mode_t v);
void screen_update();
void draw_raw_data(void * data, unsigned int width, unsigned int height);
void screen_clear();
void screen_fillrect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint8_t r, uint8_t g, uint8_t b);
void drawchar(char c, unsigned int x, unsigned int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, int scale = 2);
void drawchar_direct(char c, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, int scale);
void drawstring(char* str, unsigned int x, unsigned int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, int scale = 2);
void drawstring_direct(char * str, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, int scale);
void screen_clear(uint8_t r, uint8_t g, uint8_t b);
//void drawbitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bmp_w, unsigned int bmp_h, uint8_t *data, int bmp_bpp);
void drawbitmap(int x, int y, int w, int h, int bmp_w, int bmp_h, uint8_t *data, int bmp_bpp);
void drawbitmap_noscale(int x, int y, int w, int h, uint8_t *data, int bmp_bpp);

void screen_fillrect_direct(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint8_t r, uint8_t g, uint8_t b);

void drawgrayscalebitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint8_t* data);

video_mode_t get_video_mode();

void screen_clear_direct(uint8_t r, uint8_t g, uint8_t b);