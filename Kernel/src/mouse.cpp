#include <stdtype.h>
#include <system.h>

#include <mouse.h>

/*
namespace mouse{

	uint8_t mouseCycle = 0;
	int8_t mouseByte[3];
	signed char x;
	signed char y;

	void Handler(struct regs32* r){
		switch(mouseCycle){
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

	inline void Wait(uint8_t type){
		uint32_t timeout=100000;

		if(type == 0){
			while(timeout--){
				if((inportb(0x64)&1)==1){
					return;
				}
			}
			return;
		} else {
			while(timeout--){
				if((inportb(0x64)&2)==0){
					return;
				}
			}
			return;
		}
	}

	inline void Write(uint8_t data){
		Wait(1);
		outportb(0x64,0xD4);
		Wait(1);
		outportb(0x60,data);
	}

	uint8_t Read(){
		Wait(0);
		return inportb(0x60);
	}

	void Init(){
		uint8_t status;

		Wait(1);
		outportb(0x64,0xA8);

		Wait(1);
		outportb(0x64,0x20);
		Wait(0);
		status = (inportb(0x60) | 2);
		Wait(1);
		outportb(0x64,0x60);
		Wait(1);
		outportb(0x60,status);

		Write(0xF6);
		Read();
		Write(0xF4);
		Read();

		InstallIRQHandler(12,Handler);
	}

	Vector2 GetPos(){
		Vector2 vec;
		vec.x = x;
		vec.y = y;
	}
}
*/

unsigned char mouse_cycle=0;     //unsigned char
signed char mouse_byte[3];    //signed char
signed char mouse_x=0;         //signed char
signed char mouse_y=0;         //signed char

//Mouse functions
void mouse_handler(struct regs32 *r) //struct regs *a_r (not used but just there)
{
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inportb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=inportb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inportb(0x60);
      mouse_x=mouse_byte[1];
      mouse_y=mouse_byte[2];
      mouse_cycle=0;
      break;
  }
}

inline void mouse_wait(unsigned char a_type) //unsigned char
{
  unsigned int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(unsigned char a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outportb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outportb(0x60, a_write);
}

unsigned char mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inportb(0x60);
}

void mouse_install()
{
  unsigned char _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outportb(0x64, 0xA8);

  //Enable the interrupts
  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);

  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge

  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  InstallIRQHandler(12, mouse_handler);
}


Vector2 mouse_getpos(){
	Vector2 a;
	a.x = mouse_x;
	a.y = mouse_y;
	return a;
}
