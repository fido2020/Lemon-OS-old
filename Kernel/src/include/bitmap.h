#pragma once

#include <stdint.h>

#define BMP_COMPRESSION_RGB 0
#define BMP_COMPRESSION_RLE8 1
#define BMP_COMPRESSION_RLE4 2
#define BMP_COMPRESSION_BITFIELDS 3
#define BMP_COMPRESSION_JPEG 4
#define BMP_COMPRESSION_PNG 5
#define BMP_COMPRESSION_ALPHABITFIELDS 6
#define BMP_COMPRESSION_CMYK 11
#define BMP_COMPRESSION_CMYKRLE8 12
#define BMP_COMPRESSION_CMYKRLE4 13

typedef struct {
	char magic[2]; // Magic number = should be equivalent to "BM"
	uint32_t size; // Size of file
	uint32_t reserved; // Reserved bytes
	uint32_t offset; // Offset of pixel data
} bitmap_header_t;

typedef struct {
	uint32_t hdr_size; // The size of this header
	uint32_t width; // The width of the image in pixels
	uint32_t height; // The height of the image in pixels
	uint16_t colour_planes;// Number of colour planes (should be 1)
	uint16_t bpp; // Bits per pixel
	uint32_t compression; // Compression method
	uint32_t size; // The image size
	uint32_t hres; // The horizontal resolution of the image (pixels per metre)
	uint32_t vres; // The vertical resolution of the image (pixels per metre)
	uint32_t colour_num; // The number of colours in the colour palette 
	uint32_t colours_important; // Number of important colours - usually ignored
} bitmapinfo_header_t;