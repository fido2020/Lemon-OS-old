#include <snake.h>

#include <video.h>
#include <timer.h>
#include <keyboard.h>
#include <math.h>
#include <string.h>

bool update_ready = false;
static char buf[8];

static uint32_t ticks;

void fps_timer_tick(uint16_t hz) {
	ticks++;
	if(ticks == 1000/24){
		update_ready = true;
		ticks = 0;
	}
}

Snake::Snake(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	gridWidth = (width - width % cellSize) / cellSize;
	gridHeight = (height - height % cellSize) / cellSize;

	timer_register_callback(fps_timer_tick);

	snake.add_back({ 6, 4 });
	snake.add_back({ 5, 4 });
	snake.add_back({ 4, 4 });
	snake.add_back({ 3, 4 });

	donut.x = rand() % gridWidth;
	donut.y = rand() % gridHeight;
}

void Snake::Update()
{
	if (get_key() == '\n')
		Reset();
	
	if (gameOver) {
		return;
	}
	if (!update_ready) return;
	update_ready = false;

	switch (direction) {
	case UP:
		snake.remove_at(snake.get_length() - 1);
		snake.add_front({ snake[0].x, snake[0].y - 1 });
		if (snake[0].y < 0) {
			gameOver = true;
		}
		break;
	case DOWN:
		snake.remove_at(snake.get_length() - 1);
		snake.add_front({ snake[0].x, snake[0].y + 1 });
		if (snake[0].y >= gridHeight) {
			gameOver = true;
		}
		break;
	case LEFT:
		snake.remove_at(snake.get_length() - 1);
		snake.add_front({ snake[0].x - 1, snake[0].y});
		if (snake[0].x < 0) {
			gameOver = true;
		}
		break;
	case RIGHT:
		snake.remove_at(snake.get_length() - 1);
		snake.add_front({ snake[0].x + 1, snake[0].y});
		if (snake[0].x >= gridWidth) {
			gameOver = true;
		}
		break;
	}

	if (snake[0] == donut) {
		donut.x = rand() % gridWidth;
		donut.y = rand() % gridHeight;
		snake.add_back({ 0,0 });
	}

	for (int i = 1; i < snake.get_length(); i++) {
		if (snake[i] == snake[0]) {
			gameOver = true;
		}
	}
}

void Snake::Render()
{
	screen_fillrect(x, y, width, height, bgColour[0], bgColour[1], bgColour[2]);

	if (gameOver) {
		drawstring("Game", x + width / 2 - 48, y + height / 2 - 24, 255, 255, 255, 3);
		drawstring("Over", x + width / 2 - 48, y + height / 2, 255, 255, 255, 3);
		drawstring("Press ENTER to reset", x + width / 2 - 80,  y + height / 2 + 24, 255, 255, 255, 1);
		return;
	}

	for (int i = 0; i < snake.get_length(); i++) {
		screen_fillrect(x + snake[i].x*cellSize, y + snake[i].y*cellSize, cellSize, cellSize, fgColour[0], fgColour[1], fgColour[2]);
	}

	screen_fillrect(x + donut.x*cellSize, y + donut.y*cellSize, cellSize, cellSize, fgColour[0], fgColour[1], fgColour[2]);
	drawstring("Score:", x, y, 255, 255, 255, 1);
	drawstring(itoa(snake.get_length(),buf,10), x + 7*8, y, 255, 255, 255, 1);
}

void Snake::Reset()
{
	gameOver = false;

	for (int i = 0; i < snake.get_length(); i++)
		snake.remove_at(i);
	snake.add_back({ 6, 4 });
	snake.add_back({ 5, 4 });
	snake.add_back({ 4, 4 });
	snake.add_back({ 3, 4 });

	donut.x = rand() % gridWidth;
	donut.y = rand() % gridHeight;

	direction = RIGHT;
}

void Snake::Relocate(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Snake::OnKeyPressed(char key){
	switch (key) {
	case 'w':
		if(direction != DOWN)
			direction = UP;
		break;
	case 'a':
		if (direction != RIGHT)
			direction = LEFT;
		break;
	case 's':
		if (direction != UP)
			direction = DOWN;
		break;
	case 'd':
		if(direction != LEFT)
			direction = RIGHT;
		break;
	default:
		break;
	}
}