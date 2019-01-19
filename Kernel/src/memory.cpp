#include <memory.h>

extern "C"
void* memset(void* src, int c, size_t count)
{
	unsigned char *xs = (uint8_t*)src;

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
extern "C" void memcpy_sse2_internal(void* dest, void* src, size_t count);
void memcpy_sse2(void* dest, void* src, size_t count) {
	size_t overflow = (count % 0x10); // Amount of overflow bytes
	size_t size_aligned = (count - overflow); // Size rounded DOWN to lowest multiple of 128 bits

	memcpy_sse2_internal(dest, src, size_aligned/0x10);

	if (overflow > 0)
		memcpy(dest + size_aligned, src + size_aligned, overflow);
}