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

	char* buf = (char*)malloc(16);
	write_serial_string(itoa((long)video_buffer, buf, (int)16));

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
void screen_putpixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b) {
	if (video_mode.type != Graphical) return; // Text Mode uses characters not pixels
	uint32_t pos = y * video_mode.pitch + (x * (video_mode.bpp / 8));
	video_buffer[pos + 2] = r;
	video_buffer[pos + 1] = g;
	video_buffer[pos + 0] = b;
}

// DDraws a filled rectangle on the screen
void screen_fillrect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t pos = 0;
	for (unsigned int i = 0; i<h; i++) {
		for (unsigned int j = 0; j<w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			video_buffer[pos] = b & 255; // BLUE
			video_buffer[pos + 1] = g;  // GREEN
			video_buffer[pos + 2] = r; // RED
		}
	}
}

// Draws a filled rectangle on the screen skipping double buffer
void screen_fillrect_direct(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t pos = 0;
	for (unsigned int i = 0; i<h; i++) {
		for (unsigned int j = 0; j<w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			video_memory[pos] = b & 255; // BLUE
			video_memory[pos + 1] = g;  // GREEN
			video_memory[pos + 2] = r; // RED
		}
	}
}

// Draws specified character with specified colour at specified location using specified scale
void drawchar(char c, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
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
void drawchar_direct(char c, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
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
void drawstring(char* str, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	int xOffset = 0;
	while (*str != 0) {
		drawchar(*str, x + xOffset, y, r, g, b, scale);
		xOffset += scale * 8;
		str++;
	}
}

// Draws specified character with specified colour at specified location using specified scale,
// skipping double buffer
void drawstring_direct(char* str, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	int xOffset = 0;
	while (*str != 0) {
		drawchar_direct(*str, x + xOffset, y, r, g, b, scale);
		xOffset += scale * 8;
		str++;
	}
}

// Draws raw image data in greyscale (one channel 0-255)
void drawgrayscalebitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint8_t* data) {
	uint32_t pos = 0;
	for (unsigned int i = 0; i < h; i++)
		for (unsigned int j = 0; j < w; j++) {
			pos = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			uint8_t col = data[(i*w) + j];

			if (col != 1) { // A bit of a hack - a value of 1 = transparent
				video_buffer[pos] = col;
				video_buffer[pos + 1] = col;
				video_buffer[pos + 2] = col;
			}
		}
}

// Returns the video mode structure
video_mode_t get_video_mode()
{
	return video_mode;
}

// Draws uncompressed BMP image data (RGB)
void drawbitmap(int x, int y, int w, int h, int bmp_w, int bmp_h, uint8_t *data, int bmp_bpp)
{
	uint32_t row_size = floor((bmp_bpp*bmp_w+31)/32)*4;
	uint32_t video_mem_offset = y*video_mode.pitch;
	uint32_t bmp_offset = 0;
	uint32_t bmp_buffer_offset = 0;
	uint8_t* bmp_buffer = (uint8_t*)malloc(w * h * (video_mode.bpp / 8));

	int pixel_downscale_h = bmp_w / w; // Horizontal pixel downscale
	int pixel_downscale_v = bmp_h / h; // Vetical pixel downscale

	uint32_t pixel_size = video_mode.bpp / 8;

	if (!(pixel_downscale_h == 1 && pixel_downscale_v == 1)) {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				bmp_buffer[bmp_buffer_offset + j * pixel_size] = data[bmp_offset + j * (bmp_bpp / 8)];
				bmp_buffer[bmp_buffer_offset + j * pixel_size + 1] = data[bmp_offset + j * (bmp_bpp / 8) + 1];
				bmp_buffer[bmp_buffer_offset + j * pixel_size + 2] = data[bmp_offset + j * (bmp_bpp / 8) + 2];
			}
			bmp_offset += row_size * pixel_downscale_v;
			bmp_buffer_offset += w * pixel_size;
		}
	}
	else {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				uint16_t r = 0, g = 0, b = 0;

				for (int k = 0; k < pixel_downscale_h; k++) {
					for (int l = 0; l < pixel_downscale_v; l++) {
						b += data[bmp_offset + l * row_size + (j * pixel_downscale_h + k) * (bmp_bpp / 8)];
						g += data[bmp_offset + l * row_size + (j * pixel_downscale_h + k) * (bmp_bpp / 8) + 1];
						r += data[bmp_offset + l * row_size + (j * pixel_downscale_h + k) * (bmp_bpp / 8) + 2];
					}
				}

				b /= pixel_downscale_h;
				g /= pixel_downscale_h;
				r /= pixel_downscale_h;

				bmp_buffer[bmp_buffer_offset + j * pixel_size] = /*data[bmp_offset + j * (bmp_bpp / 8)]*/ (uint8_t)b;
				bmp_buffer[bmp_buffer_offset + j * pixel_size + 1] = /*data[bmp_offset + j * (bmp_bpp / 8) + 1]*/ (uint8_t)g;
				bmp_buffer[bmp_buffer_offset + j * pixel_size + 2] = /*data[bmp_offset + j * (bmp_bpp / 8) + 2]*/ (uint8_t)r;
			}
			bmp_offset += row_size;
			bmp_buffer_offset += w * pixel_size;
		}
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w * (video_mode.bpp / 8); j++) {
			video_buffer[(y + i)*video_mode.pitch + j + (x * (video_mode.bpp/8))] = bmp_buffer[(w * h * pixel_size) - i*(pixel_size*w) + j];
		}
	}
}

void drawbitmap_noscale(int x, int y, int w, int h, uint8_t *data, int bmp_bpp) {
	uint32_t row_size = floor((bmp_bpp*w + 31) / 32) * 4;
	uint32_t bmp_offset = row_size * (h - 1);
	uint32_t bmp_buffer_offset = 0;
	//uint8_t* bmp_buffer = (uint8_t*)malloc(w * h * (video_mode.bpp / 8));

	uint32_t pixel_size = video_mode.bpp / 8;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			uint32_t offset = (y + i)*video_mode.pitch + (x + j) * pixel_size;
			video_buffer[offset] = data[bmp_offset + j * (bmp_bpp / 8)];
			video_buffer[offset + 1] = data[bmp_offset + j * (bmp_bpp / 8) + 1];
			video_buffer[offset + 2] = data[bmp_offset + j * (bmp_bpp / 8) + 2];
		}
		bmp_offset -= row_size;
		bmp_buffer_offset += w * pixel_size;
	}

	//free(bmp_buffer);
}


// Copies the double buffer over to the framebuffer
void screen_update() {
	memcpy_sse2(video_memory, video_buffer, video_mode.pitch*video_mode.height);
}