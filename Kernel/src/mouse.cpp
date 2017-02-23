#include <stdtype.h>
#include <system.h>

#include <mouse.h>

unsigned char mouse_cycle=0;
signed char mouse_byte[3];
signed char mouse_x=0;
signed char mouse_y=0;

//Mouse functions
void MouseHandler(struct regs32 *r)
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

inline void MouseWait(unsigned char a_type)
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

inline void MouseWrite(unsigned char a_write) //unsigned char
{
  MouseWait(1);
  outportb(0x64, 0xD4);
  MouseWait(1);
  outportb(0x60, a_write);
}

unsigned char MouseRead()
{
  MouseWait(0);
  return inportb(0x60);
}

void InstallMouseHandler()
{
  unsigned char _status;

  //Enable the auxiliary mouse device
  MouseWait(1);
  outportb(0x64, 0xA8);

  //Enable the interrupts
  MouseWait(1);
  outportb(0x64, 0x20);
  MouseWait(0);
  _status=(inportb(0x60) | 2);
  MouseWait(1);
  outportb(0x64, 0x60);
  MouseWait(1);
  outportb(0x60, _status);

  //Tell the mouse to use default settings
  MouseWrite(0xF6);
  MouseRead();

  //Enable the mouse
  MouseWrite(0xF4);
  MouseRead();

  //Setup the mouse handler
  InstallIRQHandler(12, MouseHandler);
}


Vector2 GetMouseSpeed(){
	Vector2 a;
	a.x = mouse_x;
	a.y = mouse_y;
	return a;
}
