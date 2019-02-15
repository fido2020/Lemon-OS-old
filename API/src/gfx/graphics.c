#include <gfx/graphics.h>

void fillrect_s(rect_t rect, rgba_colour_t colour, surface_t* surface){
    uint32_t colour_i = (colour.a << 24) | (colour.r << 16) | (colour.g << 8) | colour.b;
    uint32_t* buffer = (uint32_t*)surface->buffer; // Convert byte array into an array of 32-bit unsigned integers as the supported colour depth is 32 bit
    for(int i = 0; i < rect.size.y; i++){
        for(int j = 0; j < rect.size.x; j++){
            buffer[(i + rect.pos.y) * surface->width + (j + rect.pos.x)] = colour_i;
        }
    }
}

void fillrect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, surface_t* surface){
    uint32_t colour_i = (r << 16) | (g << 8) | b;
    uint32_t* buffer = (uint32_t*)surface->buffer; // Convert byte array into an array of 32-bit unsigned integers as the supported colour depth is 32 bit
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            buffer[(i + y) * surface->width + (j + x)] = colour_i;
        }
    }
}