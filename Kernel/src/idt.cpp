#include <idt.h>

struct IDTEntry idt[256];
struct IDTPtr idtPtr;

void loadIDT(){
	__asm__("lidt idtPtr");
}

void IDT_SetGate(uint8_t num, uint64_t offset, uint16_t sel, uint8_t flags){
	idt[num].offset_low = (offset & 0xFFFF);
	idt[num].offset_high = (offset >> 16) & 0xFFFF;
	idt[num].selector = sel;
	idt[num].zero = 0;
	idt[num].flags = flags;
}

void initIDT(){
	idtPtr.limit = (sizeof(struct IDTEntry)*256)-1;
	idtPtr.offset = (uint32_t)&idt;
	memset(&idt,0,sizeof(struct IDTEntry)*256);

	loadIDT();
}
