#pragma once

#include <stdint.h>
#include <list.h>

#define KEY_RIGHT 0x4f // Keyboard Right Arrow
#define KEY_LEFT 0x50 // Keyboard Left Arrow
#define KEY_DOWN 0x51 // Keyboard Down Arrow
#define KEY_UP 0x52 // Keyboard Up Arrow

struct Vector2i
{
	int x, y;
};

static bool operator==(const Vector2i& l, const Vector2i& r) {
	if (l.x == r.x && l.y == r.y) return true;
	else return false;
}

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

class Snake {
public:
	Snake(int x, int y, int width, int height);
	void Update();
	void Render();
	void Reset();
	void Relocate(int x, int y);
	void OnKeyPressed(char key);
private:
	const int cellSize = 16; // Grid square size

	int x; // X pos of game render area (pixels)
	int y; // Y pos of game render area (pixels)
	int width; // Width of game render area (pixels)
	int height; // Height of game render area (pixels)
	int gridWidth; // Width in grid squares
	int gridHeight; // Height in grid squares

	uint8_t bgColour[3] = {170, 200, 170}; // Background Colour
	uint8_t fgColour[3] = {96, 96, 96}; // Foreground Colour (snake, food, etc.)

	List<Vector2i> snake;
	Vector2i donut;
	Vector2i lemon;
	char direction = RIGHT;

	bool gameOver;
};