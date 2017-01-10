#include <conio.h>
#include <textscreen.h>
#include <lemon.h>

using namespace textscreen;

namespace console{
	
	int barFg = 0x0;
	int barBg = 0x7;
	
	int consoleFg = 0xF;
	int consoleBg = 0x0;

	void print(char data){
		putc(data, (consoleBg << 4) + consoleFg);
	}
	
	void print(char* data){
		puts(data, (consoleBg << 4) + consoleFg);
	}
	
	void clear(){
		textscreen::clear(consoleBg);
		clearLine(0, barBg << 4);
		
		puts("Lemon ", (barBg << 4) + barFg);
		puts(lemon_version, (barBg << 4) + barFg);
		
		setCurPos(0,1);
	}
}