#include <textscreen.h>
#include <memory.h>
#include <system.h>
#include <stdtype.h>

namespace textscreen{

	int videoMemory = 0xB8000; // Video Memory
	
	int scrW = 80; // Screen Width
	int scrH = 25; // Screen Height
	int scrD = 2; // Screen Depth
	
	int curX = 0; // Cursor X
	int curY = 0; // Cursor Y

	// Updates text-mode cursor
	void updateCur(){
		size_t temp;

		temp = curY * scrW + curX;

		outportb(0x3D4, 14);
		outportb(0x3D5, temp >> 8);
		outportb(0x3D4, 15);
		outportb(0x3D5, temp); 
	}
	
	void setCurPos(int x, int y){
		curX = x;
		curY = y;
		updateCur();
	}
	
	void getCur(int* x, int* y){
		x = &curX;
		y = &curY;
	}
	
	// Puts a character on the screen with specified colour
	void putc(char ch, int colour){
		char* mem = (char*) videoMemory;
		switch(ch)
		{
			case (0x08):
                if(curX > 0) 
                {
	                curX--;									
					mem[(curY * scrW + curX)*scrD]=0x00;	                              
				}
				break;
			case (0x09):
				curX += 4;
				break;
			case ('\r'):
				curX = 0;
				break;
			case ('\n'):
				curX = 0;
				curY++;
				break;
			default:
				mem [((curY * scrW + curX))*scrD] = ch;
                mem [((curY * scrW + curX))*scrD+1] = colour;
				curX++; 
				break;
		
		}
		if(curX >= scrW)                                                                   
		{
			curX = 0;                                                                
			curY++;                                                                    
		}
		updateCur();
	}
	
	// Puts a string of characters on the screen with specified colour
	void puts(char* str, int colour){
		while(*str != 0){
			putc(*str, colour);
			str++;
		}
	}
	
	// Clears the screen
	void clear(){
		clear(0);
	}
	
	// Clears the screen with specified colour
	void clear(int colour){
		curX = 0;
		curY = 0;

		for(int i = 0; i < scrW * scrH; i++){
			putc(' ', colour);
		}
		
		curX = 0;
		curY = 0;
		
		updateCur();
	}
	
	void clearLine(int line, int colour){
		int tempX = curX;
		int tempY = curY;
		curX = 0;
		curY = line;
		
		for(int i = 0; i < scrW; i++)
			putc(' ', colour);
		
		curX = tempX;
		curY = tempY;
	}
}