#include "vga.h"

void drawSplash(int x, int y, int colour){
	drawrect(x,y+50,25,25,colour);
	drawrect(x+25,y+46,176,33,colour);
	drawrect(x+25+4,y+42,168,41,colour);
	drawrect(x+25+8,y+38,160,49,colour);
	drawrect(x+25+12,y+34,152,57,colour);
	drawrect(x+25+16,y+30,143,59,colour);
	drawrect(x+25+16,y+31,144,64,colour);
	drawrect(x+25+20,y+32,136,68,colour);
	drawrect(x+25+24,y+26,128,69,colour);
	drawrect(x+200,y+50,25,24,colour);
}