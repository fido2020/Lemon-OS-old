#pragma once

#include <stdint.h>

struct process;

typedef struct Vector2i{
    int x, y;
} vector2i_t; // Two dimensional integer vector

typedef struct Rect{
    vector2i_t pos;
    vector2i_t size;
} rect_t; // Rectangle

typedef struct Surface{
	int x, y, width, height; // Self-explanatory
	uint8_t depth; // Pixel depth
	uint8_t* buffer; // Start of the buffer
	void* buffer_ptr; // Pointer to the allocated memory for the buffer (For when SSE aligned)
} surface_t;

typedef struct GUIContext{
	surface_t* surface;
	process* owner;
	uint32_t owner_pid;
} gui_context_t;

bool operator==(const Vector2i& l, const Vector2i& r);
