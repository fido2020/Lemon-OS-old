#include <stdtype.h>
#include <graphics.h>

using namespace Graphics;

struct ConsoleCharacter{
	bool glyph;
	char character;
	RGBColour colour;
};

class Console{
public:
	Console(int x, int y, int width, int height, int scale = 1);

	void print(char* str);
	void print(char ch);
	//void printNum(int64_t num,uint16_t base = 10);
	void printNum(int num, uint16_t base = 10);
	void printGlyph(char glyph);

	void print(char* str,uint8_t r,uint8_t g,uint8_t b);
	void print(char ch,uint8_t r,uint8_t g,uint8_t b);
	//void printNum(int64_t num,uint8_t r,uint8_t g,uint8_t b,uint16_t base = 10);
	void printNum(int num,uint8_t r,uint8_t g,uint8_t b,uint16_t base = 10);
	void printGlyph(char glyph,uint8_t r,uint8_t g,uint8_t b);

	void clear();
	void clear(uint8_t r,uint8_t g,uint8_t b);

	void setForegroundColour(RGBColour colour);

	void scroll();
private:
	int width;
	int height;

	int widthInCharacters;
	int heightInCharacters;

	ConsoleCharacter* characters;

	int x;
	int y;

	int curX = 0;
	int curY = 0;

	int scale = 1;

	RGBColour bgColour{0,0,0};
	RGBColour fgColour{220,220,200};
};
