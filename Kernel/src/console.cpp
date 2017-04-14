#include <console.h>

Console::Console(int x, int y, int width, int height, int scale){
	this->width = width;
	this->height = height;
	this->scale = scale;

	this->x = x;

	this->y = y;

	widthInCharacters = width-(width%(8*scale))/(8*scale);
	heightInCharacters = (height-(height%(8*scale)))/(8*scale);

	characters = new ConsoleCharacter[widthInCharacters*heightInCharacters];
}

void Console::print(char* str){
	print(str,fgColour.r,fgColour.g,fgColour.b);
}

void Console::print(char* str,uint8_t r,uint8_t g,uint8_t b){
	while(*str != 0){
		print(*str);
		str++;
	}
}

void Console::print(char ch){
		print(ch,fgColour.r,fgColour.g,fgColour.b);
}

void Console::print(char ch,uint8_t r,uint8_t g,uint8_t b){
	switch(ch){
		case '\n':
			curX = 0;
			scroll();
			curY++;
			break;
		default:
			drawchar(ch,x+curX*(8*scale),y+curY*(8*scale),r,g,b,scale);
			characters[curY*widthInCharacters+curX] = {false,ch,{r,g,b}};
			scroll();
			curX++;
			break;
	}
}

/*void Console::printNum(int64_t num, uint16_t base){
	printNum(num,base,fgColour.r,fgColour.g,fgColour.b);
}

void Console::printNum(int64_t num,uint8_t r,uint8_t g,uint8_t b,uint16_t base){
	print(itoa(num,nullptr,base),r,g,b);
}*/

void Console::printNum(int num, uint16_t base){
	printNum(num,fgColour.r,fgColour.g,fgColour.b,base);
}


void Console::printNum(int num,uint8_t r,uint8_t g,uint8_t b,uint16_t base){
	print(itoa(num,nullptr,base),r,g,b);
}

void Console::printGlyph(char glyph){
	printGlyph(glyph,fgColour.r,fgColour.g,fgColour.b);
}

void Console::printGlyph(char glyph,uint8_t r, uint8_t g,uint8_t b){
	switch(glyph){
		case '\n':
			curY++;
			curX = 0;
			break;
		default:
			drawglyph(glyph,x+curX*(8*scale),y+curY*(8*scale),r,g,b,scale);
			curX++;
			break;
	}
}

void Console::clear(){
	clear(bgColour.r,bgColour.g,bgColour.b);
	print(itoa(heightInCharacters));
}

void Console::clear(uint8_t r,uint8_t g ,uint8_t b){
	fillrect(x,y,width,height,r,g,b);
	curX = 0;
	curY = 0;
	characters = new ConsoleCharacter[heightInCharacters];
}

void Console::scroll(){
	/*if(curX >= widthInCharacters-1){
		curX = 0;
		scroll();
		curY++;
	}*/
	if(curY >= heightInCharacters){
		ConsoleCharacter* prevChars = characters;
		clear();
		for(int i=1;i<heightInCharacters;i++){
			for(int j=0;j<widthInCharacters;j++){
				ConsoleCharacter character = prevChars[i*widthInCharacters+j];
				print(character.character,character.colour.r,character.colour.g,character.colour.b);

			}
		}
	} else {
		fillrect(0,0,100,100,0,0,0);
		drawstring(itoa(heightInCharacters),0,0,255,255,255);
		drawstring(itoa(curY),0,16,255,255,255);
	}

}
