#ifndef SURFACE_H
#define SURFACE_H

typedef struct Surface{
	int x, y, width, height; // Self-explanatory
	uint8_t depth; // Pixel depth
	uint8_t* buffer; // Start of the buffer
} surface_t;

#endif