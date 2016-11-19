/*int cursorX = 0;
int cursorY = 0;

int scrWidth = 80;
int scrHeight = 25;
int scrDepth = 2;

int bgColour = 0x0;
int fgColour = 0xF;

void putc(const char c){
	char* vidmem = (char*)(0xB8000);
	switch(c)
    {
		case(0x09):
			cursorX++;
			break;
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        default:
                vidmem [((cursorY * scrWidth + cursorX))*scrDepth] = c;
                vidmem [((cursorY * scrHeight + cursorX))*scrDepth+1] = bgColour+fgColour;
                cursorX++; 
                break;
	
    }
}

void putc(const char c, int colour){
	char* vidmem = (char*)(0xB8000);
	switch(c)
    {
		case(0x09):
			cursorX++;
			break;
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        default:
                vidmem [((cursorY * scrWidth + cursorX))*scrDepth] = c;
                vidmem [((cursorY * scrHeight + cursorX))*scrDepth+1] = colour;
                cursorX++; 
                break;
	
    }
}

void puts(const char* s){
	while(*s != 0){
		putc(*s);
		s++;
	}
}

void clear(int colour){
	cursorX = 0;
	cursorY = 0;
	for(int i=0;i<=scrWidth*scrHeight;i++){
		putc(' ', colour);
	}
}*/