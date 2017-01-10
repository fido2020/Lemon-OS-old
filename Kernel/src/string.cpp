#include <stdtype.h>

char* strcat(char *dest, const char *src)
{
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++);
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}

char* strncat(char *dest, int len, const char *src)
{
    size_t i,j;
    for (i = 0; i < len; i++);
    for (j = 0; j < len; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}

void reverse(char *string, int length) 
{
   int c;
   char *begin, *end, temp;

   begin  = string;
   end    = string;
 
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
 
int strlen(char *pointer)
{
   int c = 0;
 
   while( *(pointer + c) != '\0' )
      c++;
 
   return c;
}