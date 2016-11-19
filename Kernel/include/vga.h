#pragma once

enum vga_mode{
	Text40x25,
	Text80x25,
	Default,
	Graphical320x200x256,
	Graphical640x480x16,
	Graphical320x200x16
};

extern void vga_setMode(vga_mode mode);
extern void vga_setMode(int mode);

extern void clearScreen(int colour);

extern void clearLine(int line, int colour);

extern void setCursorPosition(int x, int y);

extern void putc(const char c, int colour);
extern void puts(const char* s, int colour);