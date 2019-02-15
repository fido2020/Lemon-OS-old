#include <gui.h>

bool operator==(const Vector2i& l, const Vector2i& r) {
	if (l.x == r.x && l.y == r.y) return true;
	else return false;
}