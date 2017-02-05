#include <stdtype.h>

namespace mouse{
	
	uint8_t mouseCycle = 0;
	int8_t mouseByte[3];
	int8_t x;
	int8_t y;
	
	void update(){
		switch(cycle){
			case 0:
				mouseByte[0]=inportb(0x60);
				mouseCycle++;
				break;
			case 1:
				mouseByte[1]=inportb(0x60);
				mouseCycle++;
				break;
			case 2:
				mouseByte[2] = inportb(0x60);
				x = mouseByte[1];
				y = mouseByte[2];
				break;
		}
	}
	
	inline void wait(uint8_t type){
		uint32_t timeout=100000;
		
		if(type == 0){
			while(timeout--){
				if(inportb(0x64)&1)==1){
					return;
				}
			}
			return;
		} else {
			while(timeout--){
				if(inport(0x64)&2)==0){
					return;
				} 
			}
			return;
		}
	}
	
	inline void write(uint8_t data){
		wait(1);
		outportb(0x64,0xD4);
		wait(1);
		outportb(0x60,data)
	}
	
	uint8_t read(){
		wait(0);
		return inportb(0x60);
	}
	
	void init(){
		uint8_t status;
		
		wait(1);
		outportb(0x64,0xA8);
		
		wait(1);
		outportb(0x64,0x20);
		wait(0);
		status = (inportb(0x60) | 2);
		wait(1);
		outportb(0x64,0x60);
		wait(1);
		outportb(0x60,status);
		
		write(0xF6);
		read();
	}
}