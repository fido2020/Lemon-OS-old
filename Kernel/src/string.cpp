#include <common.h>

// strcat - Concatanate strings
char* strcat(char *dest, const char *src)
{
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++);
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}

// strncat - Concatanate strings but only take specified umber of char from source
char* strncat(char *dest, int len, const char *src)
{
    size_t i,j;
    for (i = 0; i < len; i++);
    for (j = 0; j < len; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}

// reverse - Reverses a string
void reverse(char *str, int length)
{
   int c;
   char *begin, *end, temp;

   begin  = str;
   end    = str;

   for (c = 0; c < length - 1; c++)
      end++;

   for (c = 0; c < length/2; c++)
   {
      temp   = *end;
      *end   = *begin;
      *begin = temp;

      begin++;
      end--;
   }
}

// strlen - Get length of a string
int strlen(char *str)
{
   int c = 0;

   while( *(str + c) != '\0' )
      c++;

   return c;
}

// indexOf - Get index of specified char in string
int indexOf(char c, char *str){
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] == c)
			return i;
		i++;
	}
	return strlen(str);
}


void substr(int i, char *src, char *dest){ //substring exclusive
	memcpy(dest,src,i);
	dest[i] = '\0';
}

void substr(int s, int e, char *src, char *dest){ //substring exclusive range (end is exclusive, beginning is inclusive)
	memcpy(dest,&src[s],e-s);
	dest[e-s] = '\0';
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str, i);

    return str;
}
