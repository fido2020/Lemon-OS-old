#include <stdtype.h>

uint8_t inportb (uint16_t _port)
{
    	uint8_t rv;
    	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    	return rv;
}

void outportb (uint16_t _port, uint8_t _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/*void outportb(uint32_t ad,uint8_t v){
	asm volatile("outb %%al, %%dx" :: "d" (ad), "a" (v));
}

uint8_t inportb(uint16_t ad){
	uint8_t _v;
	__asm__ __volatile__("inb %%dx, %%al" : "=a" (_v) : "d" (ad));
	return _v;
}*/