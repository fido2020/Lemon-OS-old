#pragma once

enum vga_mode{
	Text40x25,
	Text80x25,
	Default,
	Graphical320x200x256,
	Graphical640x480x16,
	Graphical320x200x16
};

extern void init_vga(vga_mode mode);
extern void init_vga(int mode);

extern void clear(int colour);