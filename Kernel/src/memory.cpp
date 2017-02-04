#include <conio.h>

#include <memory.h>


unsigned char *memset(unsigned char *src, int c, size_t count)
{
    unsigned char *xs = src;
	
    while (count--)
        *xs++ = c;
    return src;
}
