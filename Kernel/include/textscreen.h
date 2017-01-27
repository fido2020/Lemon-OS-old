#pragma once

#include <stdtype.h>

namespace textscreen{

	void updateCur();
	void setCurPos(uint16_t x, uint16_t y);
	
	void putc(char ch, uint8_t colour);
	void puts(char* str, uint8_t colour);
	
	void clear();
	void clear(uint8_t colour);
	void clearLine(uint8_t line, uint8_t colour);
	
	void setLineScrollPosition(uint8_t line);
	
	uint16_t getCurX();
	uint16_t getCurY();
	
	void scrollUp(uint8_t colour);
}