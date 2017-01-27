#include <conio.h>
#include <textscreen.h>
#include <lemon.h>
#include <stdtype.h>

using namespace textscreen;

namespace console{
	
	uint8_t barFg = 0x0;
	uint8_t barBg = 0x7;
	
	uint8_t consoleFg = 0xF;
	uint8_t consoleBg = 0x0;

	void print(char data){
		putc(data, (consoleBg << 4) + consoleFg);
	}
	
	void print(char* data){
		puts(data, (consoleBg << 4) + consoleFg);
	}
	
	void clear(){
		textscreen::clear(consoleBg);
		/*clearLine(0, barBg << 4);
		
		puts("Lemon ", (barBg << 4) + barFg);
		puts(Lemon::version, (barBg << 4) + barFg);
		
		setCurPos(0,1);*/
	}

	void panic(char* error, char* msg, bool hang){
		clear();
		print("PANIC: ");
		print(error);
		print('\n');
		print(msg);
		print("\n\n");
		
		print("Version: ");
		print(Lemon::version);

		if(hang){
			for(;;);
		}
	}
}

// Panic needs to work in c as well
extern "C"
void cpanic(char* error, char* msg, bool hang){
	console::clear();
	console::print("PANIC: ");
	console::print(error);
	console::print('\n');
	console::print(msg);
	if(hang){
		for(;;);
	}
}