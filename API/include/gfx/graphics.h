#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <gfx/surface.h>

typedef struct Vector2i{
    int x, y;
} vector2i_t; // Two dimensional integer vector

typedef struct Rect{
    vector2i_t pos;
    vector2i_t size;
} rect_t; // Rectangle

typedef struct RGBAColour{
    uint8_t r, g, b, a; /* Red, Green, Blue, Alpha (Transparency) Respectively*/
} rgba_colour_t;

// fillrect (rect, colour, surface*) - Draw filled rectangle
void fillrect_s(rect_t rect, rgba_colour_t colour, surface_t* surface);
void fillrect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, surface_t* surface);

#endif