#include <stdint.h>
#include <memory.h>
#include <int32.h>

#include <vga.h>

int textModeCursorX = 0;
int textModeCursorY = 0;

int consoleBgColour = 0;
int consoleFgColour = 15;

bool graphical = false;

int screenWidth;
int screenHeight;

int textModeScreenDepth = 2;

char* textMemory = (char*)0xB8000;
char* graphicalMemory = (char*)0xA0000;

char* videoMemory = textMemory;

void vga_setMode(vga_mode mode){
	int y;
	regs16_t regs;
	
	
	regs.ax = 0x03;
	
	if(mode == Text80x25 || mode == Default){
		regs.ax = 0x03;
		screenWidth = 80;
		screenHeight = 25;
		videoMemory = textMemory;
	} else if(mode == Text40x25){
		regs.ax = 0x01;
		screenWidth = 40;
		screenHeight = 25;
		videoMemory = textMemory;
	} else if(mode == Graphical320x200x256){
		regs.ax = 0x13;
		screenWidth = 320;
		screenHeight = 200;
		videoMemory = graphicalMemory;
	} else if(mode == Graphical320x200x16){
		regs.ax = 0x0D;
		screenWidth = 320;
		screenHeight = 200;
		videoMemory = graphicalMemory;
	} else if(mode == Graphical640x480x16){
		regs.ax = 0x12;
		screenWidth = 640;
		screenHeight = 480;
		videoMemory = graphicalMemory;
	}
	int32(0x10, &regs);
}

void vga_setMode(int mode){
	int y;
	regs16_t regs;
	
	regs.ax = mode;
	int32(0x10, &regs);
}

void clearScreen(int colour){
	if(graphical)
		memset((char*)videoMemory, colour, (screenWidth*screenHeight));
	else {
		textModeCursorX = 0;
		textModeCursorY = 0;
		for(int i=0;i<=screenWidth*screenHeight;i++){
			putc(' ', colour);
		}
	}
	
}

// Text Mode
	void setCursorPosition(int x, int y){
		textModeCursorX = x;
		textModeCursorY = y;
	}

	void clearLine(int line, int colour){
		textModeCursorY = line;
		textModeCursorX = 0;
		for(int i=0;i<screenWidth;i++)
			putc(' ', colour);
	}

	void puts(const char* s, int colour){
		while(*s != 0){
			putc(*s, colour);
			s++;
		}
	}
	
	void puts(const char* s){
		while(*s != 0){
			putc(*s);
			s++;
		}
	}
	
	void putc(const char c, int colour){
		switch(c)
		{
			case(0x09):
				textModeCursorX++;
				break;
			case ('\r'):
					textModeCursorX = 0;
					break;
			case ('\n'):
					textModeCursorX = 0;
					textModeCursorY++;
					break;
			default:
					videoMemory [((textModeCursorY * screenWidth + textModeCursorX))*textModeScreenDepth] = c;
					videoMemory [((textModeCursorY * screenHeight + textModeCursorX))*textModeScreenDepth+1] = colour;
					textModeCursorX++; 
					break;
		
		}
	}

	void putc(const char c){
		switch(c)
		{
			case(0x09):
				textModeCursorX++;
				break;
			case ('\r'):
					textModeCursorX = 0;
					break;
			case ('\n'):
					textModeCursorX = 0;
					textModeCursorY++;
					break;
			default:
					videoMemory [((textModeCursorY * screenWidth + textModeCursorX))*textModeScreenDepth] = c;
					videoMemory [((textModeCursorY * screenHeight + textModeCursorX))*textModeScreenDepth+1] = consoleBgColour + consoleFgColour;
					textModeCursorX++; 
					break;
		
		}
	}

// Graphical Mode

void setPixel(int x, int y, int colour){
	if(graphical){
		if(y == 0){
			memset((char*)videoMemory + x, colour, 1);
		} else {
			memset((char*)videoMemory + ((y)*screenWidth + x), colour, 1);
		}
	} else {
		
	}
}

void drawRect(int x, int y, unsigned char w, unsigned char h, int colour) {
	if(graphical){
		if((h+y)>screenHeight){
			while(h+y > screenHeight){
				h--;
			}
		}
		for(int i=y;i<h+y;i++)
			memset((char *)videoMemory + ((i)*screenWidth + x), colour, w);
	} else {
		
	}
}