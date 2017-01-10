void *memset(void *s, int c, int count)
{
    char *xs = s;
	
    while (count--)
        *xs++ = c;
    return s;
}