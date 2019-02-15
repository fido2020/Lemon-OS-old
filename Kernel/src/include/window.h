#pragma once

#include <stdint.h>
#include <console.h>
#include <list.h>
#include <widget.h>
#include <gui.h>

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
#define WINDOW_FLAGS_

enum WindowType {
	windowtype_gui, // GUI window with widgets
	windowtype_console, // Console window
	windowtype_framebuffer, // Raw framebuffer window
};

enum EventType{
	event_empty,
	event_key,
	event_mouse,
};

class Event{
public:
	uint8_t type;
	void* data;
	~Event(){
		free(data);
	}
};

struct KeyEventData{
	char key;
};

struct MouseEventData{
	int x;
	int y;
	int button;
};

struct process;

typedef struct {
	uint16_t x = 0;
	uint16_t y = 0;

	uint16_t width = 0; // Width
	uint16_t height = 0; // Height

	uint32_t flags; // Window Flags

	char title[96]; // Title of window
} __attribute__((packed)) win_info_t;

struct Desktop;

typedef struct Window{
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;

	uint32_t flags;

	uint8_t* buffer;

	Desktop* desktop;

	char title[96];
} window_t;

typedef struct Desktop{
	List<window_t*>* windows;

	surface_t surface;
} __attribute__((packed)) desktop_t;

void set_main_desktop(desktop_t* desktop);
desktop_t* get_main_desktop();