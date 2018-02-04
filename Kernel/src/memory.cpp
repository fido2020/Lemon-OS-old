#include <memory.h>

extern "C"
unsigned char *memset(uint8_t* src, int c, size_t count)
{
	unsigned char *xs = src;

	while (count--)
		*xs++ = c;
	return src;
}