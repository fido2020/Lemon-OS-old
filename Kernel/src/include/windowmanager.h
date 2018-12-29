#pragma once

#include <stdint.h>
#include <mischdr.h>
#include <list.h>
#include <window.h>

typedef struct {
	int left, right, bottom, top;
} rect_t;

class WindowManager
{
public:
	// Constructor
	WindowManager(video_mode_t * video_mode);

	// Create a new window
	Window* Window_New(int x, int y, int width, int height, uint8_t type);

	// Render (called every frame)
	void Render();
	// Update (called every frame)
	void Update();

	// Return wm_lock
	bool IsLocked();

	// Mouse cursor X position
	unsigned int mouse_x = 250;
	// Mouse cursor Y position
	unsigned int mouse_y = 250;

	bool mouseDown = false;

private:
	// Add existing window
	void Window_Add(Window* win);

	// Linked list containing the windows
	List<Window*> windows;
	// Number of windows
	int num_windows = 0;

	// Window currently in focus/active
	Window* activeWindow;
	// Is the user dragging a window?
	bool drag = false;
	int drag_offset_x;
	int drag_offset_y;

	// Screen Width (in pixels)
	uint32_t screen_width;
	// Screen Height (in pixels)
	uint32_t screen_height;

	// Video mode data structure
	video_mode_t video_mode;

	// Draw/Render the mouse cursor
	void DrawMouseCursor();

	// Handle a mouse click
	void MouseClick();
	bool wm_lock = false;
};

WindowManager* wm_getinstance();