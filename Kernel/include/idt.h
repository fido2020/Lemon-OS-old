#include <stdtype.h>

struct IDTEntry{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
};

void LoadIDT();