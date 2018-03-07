#include <memory.h>

extern "C"
unsigned char *memset(uint8_t* src, int c, size_t count)
{
	unsigned char *xs = src;

	while (count--)
		*xs++ = c;
	return src;
}

void *memcpy(void* dest, void* src, size_t count) {
	const char *sp = (char *)src;
	char *dp = (char *)dest;
	for (size_t i = 0; i<count; i++) dp[i] = sp[i];
	return dest;
}