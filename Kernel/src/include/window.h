#pragma once

#include <stdint.h>
#include <console.h>
#include <list.h>
#include <widget.h>

#define WINDOW_DEFAULT_BG_COLOUR_R 225
#define WINDOW_DEFAULT_BG_COLOUR_G 225
#define WINDOW_DEFAULT_BG_COLOUR_B 225

#define WINDOW_TITLEBAR_ACTIVE_COLOUR_R 64
#define WINDOW_TITLEBAR_ACTIVE_COLOUR_G 64
#define WINDOW_TITLEBAR_ACTIVE_COLOUR_B 64

#define WINDOW_TITLEBAR_COLOUR_R 180
#define WINDOW_TITLEBAR_COLOUR_G 180
#define WINDOW_TITLEBAR_COLOUR_B 180

#define WINDOW_FLAGS_NODECORATION 0x1

enum WindowType {
	windowtype_gui, // GUI window with widgets
	windowtype_console, // Console window
	windowtype_framebuffer, // Raw framebuffer window
};

class Window {
public:
	uint16_t x = 0;
	uint16_t y = 0;

	uint16_t width = 0;
	uint16_t height = 0;

	uint32_t r, g, b;

	uint32_t flags;

	uint8_t type;
	Console* console;

	bool active;

public:
	Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t type = windowtype_gui);
	Window();
	void Render();
	void(*render_callback)(void) = 0;
	void(*mouse_callback)(int,int) = 0;
};