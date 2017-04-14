#include <stdtype.h>
#include <system.h>

#include <mouse.h>

unsigned char mouse_cycle=0;
signed char mouse_byte[3];
signed char mouse_x=0;
signed char mouse_y=0;

//Mouse functions
void mouseHandler(struct regs32 *r)
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

inline void mouseWait(unsigned char a_type)
{
  unsigned int _time_out=100000;
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

inline void mouseWrite(unsigned char a_write) //unsigned char
{
  mouseWait(1);
  outportb(0x64, 0xD4);
  mouseWait(1);
  outportb(0x60, a_write);
}

unsigned char mouseRead()
{
  mouseWait(0);
  return inportb(0x60);
}

void installMouseHandler()
{
  unsigned char _status;

  //Enable the auxiliary mouse device
  mouseWait(1);
  outportb(0x64, 0xA8);

  //Enable the interrupts
  mouseWait(1);
  outportb(0x64, 0x20);
  mouseWait(0);
  _status=(inportb(0x60) | 2);
  mouseWait(1);
  outportb(0x64, 0x60);
  mouseWait(1);
  outportb(0x60, _status);

  //Tell the mouse to use default settings
  mouseWrite(0xF6);
  mouseRead();

  //Enable the mouse
  mouseWrite(0xF4);
  mouseRead();

  //Setup the mouse handler
  installIRQHandler(12, mouseHandler);
}


Vector2 getMouseSpeed(){
	Vector2 a;
	a.x = mouse_x;
	a.y = mouse_y;
	return a;
}
