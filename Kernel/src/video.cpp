#include <video.h>
#include <vga.h>
#include <graphicsresources.h>
#include <memory.h>
#include <serial.h>
#include <string.h>
#include <math.h>

video_mode_t video_mode;

uint8_t* video_buffer;
uint8_t* video_memory;

// Initialize Graphics Driver
void video_initialize(video_mode_t v) {
	video_mode = v;

	video_memory = (uint8_t*)v.address;
	video_buffer = (uint8_t*)malloc(v.pitch*v.height+8);

	write_serial_string("Graphics Double Buffer allocated at 0x");
	write_serial_string(itoa((uint32_t)video_buffer, nullptr, 16));

}

// Clear the screen
void screen_clear() {
	if (video_mode.type == Text) {
		VGA::clearscreen();
	}
	else if (video_mode.type == Graphical) {
		for (uint32_t w = 0; w < video_mode.height * video_mode.pitch; w++)
			video_buffer[w] = 0;
	}
}

// Clears the screen with specified colour
void screen_clear(uint8_t r, uint8_t g, uint8_t b) {
	if (video_mode.type != Graphical) return; // Text Mode is only 16-colors
	screen_fillrect(0, 0, video_mode.width, video_mode.height, r, g, b);
}

// Clears the screen with specified colour and skips doublebuffer
void screen_clear_direct(uint8_t r, uint8_t g, uint8_t b) {
	if (video_mode.type != Graphical) return; // Text Mode is only 16-colors
	screen_fillrect_direct(0, 0, video_mode.width, video_mode.height, r, g, b);
}

// Clears the screen with specified colour
void screen_clear(uint8_t colour) {
	if (video_mode.type != Text) return; // Text Mode is the only 16-color Mode
	VGA::clearscreen(colour);
}

// Puts a pixel with specified colour at specified xy location
void screen_putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	if (video_mode.type != Graphical) return; // Text Mode uses characters not pixels
	uint32_t pos = y * video_mode.pitch + (x * (video_mode.bpp / 8));
	video_buffer[pos + 2] = r;
	video_buffer[pos + 1] = g;
	video_buffer[pos + 0] = b;
}

// DDraws a filled rectangle on the screen
void screen_fillrect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t pos = 0;
	for (int i = 0; i<h; i++) {
		for (int j = 0; j<w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			video_buffer[pos] = b & 255; // BLUE
			video_buffer[pos + 1] = g;  // GREEN
			video_buffer[pos + 2] = r; // RED
		}
	}
}

// Draws a filled rectangle on the screen skipping double buffer
void screen_fillrect_direct(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t pos = 0;
	for (int i = 0; i<h; i++) {
		for (int j = 0; j<w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			video_memory[pos] = b & 255; // BLUE
			video_memory[pos + 1] = g;  // GREEN
			video_memory[pos + 2] = r; // RED
		}
	}
}

// Draws specified character with specified colour at specified location using specified scale
void drawchar(char c, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	for (int i = 0; i < 8; i++)
	{
		int row = font_default[(int)c][i];
		for (int j = 0; j < 8; j++)
		{
			if ((row & (1 << j)) >> j)
				screen_fillrect(x + j * scale, y + i * scale, scale, scale, r, g, b);
		}
	}
}

// Draws specified character with specified colour at specified location using specified scale,
// skipping double buffer
void drawchar_direct(char c, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	for (int i = 0; i < 8; i++)
	{
		int row = font_default[(int)c][i];
		for (int j = 0; j < 8; j++)
		{
			if ((row & (1 << j)) >> j)
				screen_fillrect_direct(x + j * scale, y + i * scale, scale, scale, r, g, b);
		}
	}
}

// Draws specified string with specified colour at specified location using specified scale
void drawstring(char* str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	int xOffset = 0;
	while (*str != 0) {
		drawchar(*str, x + xOffset, y, r, g, b, scale);
		xOffset += scale * 8;
		str++;
	}
}

// Draws specified character with specified colour at specified location using specified scale,
// skipping double buffer
void drawstring_direct(char* str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	int xOffset = 0;
	while (*str != 0) {
		drawchar_direct(*str, x + xOffset, y, r, g, b, scale);
		xOffset += scale * 8;
		str++;
	}
}

// Draws raw image data in greyscale (one channel 0-255)
void drawgrayscalebitmap(int x, int y, int w, int h, uint8_t* data) {
	uint32_t pos = 0;
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			uint8_t col = data[(i*w) + j];

			if (col != 1) { // A bit of a hack - a value of 1 = transparent
				video_buffer[pos] = col;
				video_buffer[pos + 1] = col;
				video_buffer[pos + 2] = col;
			}
		}
}

void drawgrayscalebitmap(int x, int y, int w, int h, uint8_t* data, int scale) {
	uint32_t pos = 0;
	for (int i = 0; i < h * scale; i++)
		for (int j = 0; j < w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			uint8_t col = data[(i*w) + j];

			if (col != 1) { // A bit of a hack - a value of 1 = transparent
				for (int k = 0; k < scale; k++) {
					video_buffer[pos*scale + k*4] = col;
					video_buffer[pos*scale + k*4 + 1] = col;
					video_buffer[pos*scale + k*4 + 2] = col;
				}
			}
		}
}

// Returns the video mode structure
video_mode_t get_video_mode()
{
	return video_mode;
}

// Draws uncompressed BMP image data (RGB)
void drawbitmap(int x, int y, int w, int h, uint8_t *data, int bmp_bpp)
{
	uint32_t row_size = floor((bmp_bpp*w+31)/32)*4;
	write_serial_string("\r\n Row Size:");
	write_serial_string(itoa(row_size));
	uint32_t video_mem_offset = y*video_mode.pitch;
	uint32_t bmp_offset = 0;
	uint32_t bmp_buffer_offset = 0;
	uint8_t* bmp_buffer = (uint8_t*)malloc(w * h * (video_mode.bpp / 8));

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			bmp_buffer[bmp_buffer_offset + j * (video_mode.bpp / 8)] = data[bmp_offset + j * (bmp_bpp / 8)];
			bmp_buffer[bmp_buffer_offset + j * (video_mode.bpp / 8) + 1] = data[bmp_offset + j * (bmp_bpp / 8) + 1];
			bmp_buffer[bmp_buffer_offset + j * (video_mode.bpp / 8) + 2] = data[bmp_offset + j * (bmp_bpp / 8) + 2];
		}
		bmp_offset += row_size;
		bmp_buffer_offset += w * (video_mode.bpp / 8);
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w * (video_mode.bpp / 8); j++) {
			video_memory[(y + i)*video_mode.pitch + j + (x * (video_mode.bpp/8))] = bmp_buffer[i*((video_mode.bpp/8)*w) + j];
		}
	}
}


// Copies the double buffer over to the framebuffer
void screen_update() {
	memcpy(video_memory, video_buffer, video_mode.pitch*video_mode.height);
}