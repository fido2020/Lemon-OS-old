#include <windowmanager.h>

#include <mouse.h>
#include <video.h>
#include <graphicsresources.h>
#include <stddef.h>
#include <memory.h>
#include <console.h>
#include <serial.h>
#include <string.h>
#include <initrd.h>
#include <bitmap.h>
#include <timer.h>
#include <runtime.h>
#include <keyboard.h>

bool prevMouseButtonState = false;
static int8_t* mouse_data;
bool enableFpsCounter = true;

// FPS counter
uint32_t fps;

// Frames since last timer update
uint64_t frames;

uint8_t ticks = 0;

WindowManager* wminstance;

WindowManager* wm_getinstance(){
	return wminstance;
}

// Timer callback to update fps counter
void fps_update(uint16_t timer_Hz) {
	ticks++;
	if(ticks == timer_Hz){
		fps = frames/* * timer_Hz*/;
		frames = 0;
	}
}

bool WindowManager::IsLocked(){
	return wm_lock;
}

// Constructor
WindowManager::WindowManager(video_mode_t* _video_mode)
{
	if(wminstance) delete wminstance;
	wminstance = this;

	video_mode = *_video_mode;

	screen_width = video_mode.width;
	screen_height = video_mode.height;

	if(enableFpsCounter)
		timer_register_callback(fps_update);
}

// Create a new window
Window* WindowManager::Window_New(int x, int y, int width, int height, uint8_t type) {
	wm_lock = true;
	/*Window* win = (Window*)malloc(sizeof(Window));
	*win = Window(x, y, width, height, type);*/
	Window* win = new Window(x,y,width,height,type);
	windows.add_back(win);
	num_windows++;
	if (activeWindow) activeWindow->active = false;
	activeWindow = win;
	win->active = true;
	wm_lock = false;
	return win;
}
void WindowManager::Window_Add(Window* win) {
	windows.add_back(win);
	num_windows++;
}

// Render (called every frame)
void WindowManager::Render()
{
	//screen_clear(0, 0, 0);

	//screen_clear(96, 192, 192);
	Window* current;
	for (int i = 0; i < num_windows; i++) {
		if (windows[i] != NULL) {
			windows[i]->Render();
			if (windows[i]->render_callback && windows[i]->type == windowtype_framebuffer) {
				windows[i]->render_callback(windows[i]->x,windows[i]->y);
			}
			/*else if (windows[i]->type == windowtype_gui && windows[i]->widgets.num > 0) {
				for (int j = 0; j < windows[i]->widgets.num; j++) {
					windows[i]->widgets[j]->Render();
				}
			}*/
			
		}
	}

	DrawMouseCursor();
}

// Update (called every frame)
void WindowManager::Update() {
	mouse_data = mouse_get_data();
	if (mouse_data_updated()) {
		mouse_x += mouse_data[1] * 2;
		mouse_y -= mouse_data[2] * 2;

		if (mouse_data[0] & MOUSE_BUTTON_LEFT) {
			mouseDown = true;
			if (!prevMouseButtonState) {
				MouseClick();
			}
		}
		else {
			mouseDown = false;
			drag = false;
		}
	}

	if (drag && activeWindow) {
		if ((int)mouse_x - drag_offset_x >= 0)
			activeWindow->x = mouse_x - drag_offset_x;
		else
			activeWindow->x = 0;

		if((int)mouse_y - drag_offset_y >= 0)
			activeWindow->y = mouse_y - drag_offset_y;
		else activeWindow->y = 0;
	}

	if(key_updated() && activeWindow){
		Event* e = new Event();
		e->type = event_key;
		e->data = malloc(sizeof(KeyEventData));
		((KeyEventData*)e->data)->key = get_key();
		activeWindow->event_stack->add_back(e);
	}

	prevMouseButtonState = mouse_data[0] & MOUSE_BUTTON_LEFT;
	Render();

	if (enableFpsCounter) {
		// Draw FPS counter
		drawstring("FPS:", 0, 0, 255, 255, 255, 1);
		drawstring(itoa(fps), 40, 0, 255, 255, 255, 1);

		// Frames since last timer update
		frames++;
	}
}

// Draws the mouse cursor
void WindowManager::DrawMouseCursor() {
	drawgrayscalebitmap(mouse_x, mouse_y, 11, 18, mouse_cursor);
}

// Handles a mouse click
void WindowManager::MouseClick() {

	// Loop through windows
	for (int i = num_windows - 1; i >= 0; i--) {
		Window* win =  windows[i];

		// Check if mouse is within bounds of window win
		if (mouse_x >= win->x && mouse_x < (win->x + win->width) && mouse_y >= win->y && mouse_y < (win->y + win->height)) {
			if (activeWindow) activeWindow->active = false;
			activeWindow = win;
			win->active = true;
			if (i != num_windows - 1) {
				windows.remove_at(i);
				windows.add_back(win);
			}
			if ((int)mouse_y < win->y + 24) {
				if ((int)mouse_x > win->x + win->width - 20) {
					windows[i]->exists = false;
					windows.remove_at(i);
					num_windows--;
				}
				drag_offset_x = mouse_x - win->x;
				drag_offset_y = mouse_y - win->y;
				drag = true;
			}
			if (win->mouse_callback)
				win->mouse_callback(mouse_x, mouse_y);
			break;
		}
	}
}