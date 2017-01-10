#pragma once

namespace textscreen{

	void updateCur();
	void setCurPos(int x, int y);
	
	void putc(char ch, int colour);
	void puts(char* str, int colour);
	
	void clear();
	void clear(int colour);
	void clearLine(int line, int colour);
}