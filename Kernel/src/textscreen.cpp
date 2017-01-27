#include <common.h>

#include <textscreen.h>

namespace textscreen{

	int videoMemory = KERNEL_VIRTUAL_BASE + 0xB8000; // Video Memory
	
	int scrW = 80; // Screen Width
	int scrH = 25; // Screen Height
	int scrD = 2; // Screen Depth
	
	uint16_t curX = 0; // Cursor X
	uint16_t curY = 0; // Cursor Y
	
	uint8_t lineScrollPosition = 0;

	// Updates text-mode cursor
	void updateCur(){
		size_t temp;

		temp = curY * scrW + curX;

		outportb(0x3D4, 14);
		outportb(0x3D5, temp >> 8);
		outportb(0x3D4, 15);
		outportb(0x3D5, temp); 
	}
	
	void setCurPos(uint16_t x, uint16_t y){
		curX = x;
		curY = y;
		updateCur();
	}
	
	uint16_t getCurX(){
		return curX;
	}
	
	uint16_t getCurY(){
		return curY;
	}
	
	void setLineScrollPosition(uint8_t line){
		lineScrollPosition = line;
	}
	
	// Puts a character on the screen with specified colour
	void putc(char ch, uint8_t colour){
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
		if(curY >= scrH-1)
			scrollUp(colour);
		updateCur();
	}
	
	// Puts a string of characters on the screen with specified colour
	void puts(char* str, uint8_t colour){
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
	void clear(uint8_t colour){
		char* mem = (char*) videoMemory;
		
		curX = 0;
		curY = 0;

		for(int i = 0; i < scrW * scrH; i++){
			mem [i*scrD] = 219;
            mem [i*scrD+1] = (colour << 8) + colour;
		}
		
		curX = 0;
		curY = 0;
		
		updateCur();
	}
	
	void clearLine(uint8_t line, uint8_t colour){
		uint16_t tempX = curX;
		uint16_t tempY = curY;
		curX = 0;
		curY = line;
		
		for(int i = 0; i < scrW; i++)
			putc(' ', colour);
		
		curX = tempX;
		curY = tempY;
	}
	
	void scrollUp(uint8_t colour)
	{
		char* vidmem = (char*)videoMemory;
		uint16_t i = scrW*scrD;
		while(i < scrW*scrH*scrD){
			vidmem[i-(scrW*scrD)] = vidmem[i];
			i++;
		}
		i = scrW*scrD*scrH-1;
		while(i < scrW*scrH*scrD){
			vidmem[i++] = ' ';
			vidmem[i++] = colour;
		}
		curY--;
	}
}