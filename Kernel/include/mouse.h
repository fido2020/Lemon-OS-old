#include "int32.h"
#include "vga.h"

regs16_t regs;
int x;
int y;

void mouse_init(){
	regs.ax = 0;
	int32(0x33, &regs);
}

void mouse_update(){
	x=160;
	y=100;
	while (x>0 || y>0)
	{
	  /*... display mouse here ...*/
	  drawrect(x,y,2,2,0);

	  regs.ax=0x0B;
	  int32(0x33,&regs);  
	  x += (int)regs.cx;
	  y += (int)regs.dx;    
	  if (x<0) x=0;
	  if (y<0) y=0;
	  if (x>319) x=319;
	  if (y>199) y=199;
	}
}