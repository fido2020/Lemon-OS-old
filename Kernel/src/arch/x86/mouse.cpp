#include <mouse.h>
#include <idt.h>
#include <stddef.h>

int8_t mouse_data[3];
uint8_t mouse_cycle;

bool data_updated = false;

void mouse_handler(regs32_t* regs) {
	switch (mouse_cycle)
	{
	case 0:
		mouse_data[0] = inportb(0x60);
		mouse_cycle++;
		break;
	case 1:
		mouse_data[1] = inportb(0x60);
		mouse_cycle++;
		break;
	case 2:
		mouse_data[2] = inportb(0x60);
		mouse_cycle = 0;
		data_updated = true;
		break;
	}
}

inline void mouse_wait(uint8_t type) //unsigned char
{
	int timeout = 100000; //unsigned int
	if (type == 0) {
		while (timeout--) //Data
			if ((inportb(0x64) & 1) == 1)
				return;
	} else
		while (timeout--) //Signal
			if ((inportb(0x64) & 2) == 0)
				return;
}

inline void mouse_write(uint8_t data) //unsigned char
{
	mouse_wait(1);
	outportb(0x64, 0xD4);
	mouse_wait(1);
	//Send data
	outportb(0x60, data);
}

uint8_t mouse_read()
{
	//Get's response from mouse
	mouse_wait(0);
	return inportb(0x60);
}

void mouse_install()
{
	uint8_t status;  //unsigned char

				   //Enable the auxiliary mouse device
	mouse_wait(1);
	outportb(0x64, 0xA8);

	//Enable the interrupts
	mouse_wait(1);
	outportb(0x64, 0x20);
	mouse_wait(0);
	status = (inportb(0x60) | 2);
	mouse_wait(1);
	outportb(0x64, 0x60);
	mouse_wait(1);
	outportb(0x60, status);

	mouse_write(0xF6);
	mouse_read();

	mouse_write(0xF4);
	mouse_read();

	interrupt_register_handler(IRQ0 + 12, mouse_handler);
}

int8_t* mouse_get_data() {
	return mouse_data;
}

bool mouse_data_updated() {
	bool updated = data_updated;
	if(data_updated)
		data_updated = false;
	return updated;
}