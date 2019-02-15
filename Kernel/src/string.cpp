#include <stddef.h>
#include <string.h>
#include <memory.h>

string::string()
{
	length = 0;
	data = (char*)malloc(0);
}

string::string(char c)
{
	length = 1;
	data = (char*)malloc(1);
	data[0] = c;
}

string::string(const char* c)
{
	if (c)
	{
		unsigned n = 0;
		while (c[n] != '\0') n++;
		length = n;
		data = (char*)malloc(n);
		for (unsigned j = 0; j < n; j++)
			data[j] = c[j];
	}
	else
	{
		length = 0;
		data = (char*)malloc(0);
	}
}

string::string(const string& s)
{
	length = s.len();
	data = (char*)malloc(length);
	for (unsigned j = 0; j < length; j++)
		data[j] = s[j];
}



string::~string()
{
	free(data);
}


char* string::c_str() const {
	return data;
}

size_t string::len() const {
	return length;
}

char  string::operator[] (unsigned j) const {
	if (j >= length) return 0;
	return data[j];
}

char& string::operator[] (unsigned j) {
	if (j >= length) return data[0];
	return data[j];
}

string& string::operator= (const string& s)
{
	if (this == &s) return *this;

	free(data);
	length = s.len();
	data = (char*)malloc(length);
	for (unsigned j = 0; j < length; j++)
		data[j] = s[j];
	return *this;
}

string& string::operator+= (const string& s) {
	unsigned len = length + s.len();
	char* str = (char*)malloc(len);

	for (unsigned j = 0; j < length; j++)
		str[j] = data[j];

	for (unsigned i = 0; i < s.len(); i++) {
		str[length + i] = s[i];
		data[length + i] = s[i];
	}

	length = len;
	free(data);
	data = str;
	return *this;
}

string operator+ (const string& left, const string& right)
{
	return string(left) += right;
}

string operator+ (const string& left, char right)
{
	return string(left) += string(right);
}

string operator+ (const string& left, const char* right)
{
	return string(left) += string(right);
}

string operator+ (char left, const string& right)
{
	return string(left) += right;
}
string operator+ (const char* left, const string& right)
{
	return string(left) += right;
}

// strcat - Concatanate strings
extern "C"
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
    int i,j;
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

   while( str[c] != '\0' )
      c++;

   return c;
}

// indexOf - Get index of specified char in string
int strindex(char c, char *str){
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] == c)
			return i;
		i++;
	}
	return strlen(str);
}

// strchr - Get pointer to first occurance of c in string s
char *strchr(const char *s, int c)
{
	while (*s != (char)c)
		if (!*s++)
			return 0;
	return (char *)s;
}

// strspn - Get initial length of s1 including only the characters of s2
size_t strspn(const char *s1, const char *s2)
{
	size_t ret = 0;
	while (*s1 && strchr(s2, *s1++))
		ret++;
	return ret;
}

// strspn - Get initial length of s1 excluding the characters of s2
size_t strcspn(const char *s1, const char *s2)
{
	size_t ret = 0;
	while (*s1)
		if (strchr(s2, *s1))
			return ret;
		else
			s1++, ret++;
	return ret;
}

// strtok - breaks str into tokens using specified delimiters
char *strtok(char * str, const char * delim)
{
	static char* p = 0;
	if (str)
		p = str;
	else if (!p)
		return 0;
	str = p + strspn(p, delim);
	p = str + strcspn(str, delim);
	if (p == str)
		return p = 0;
	p = *p ? *p = 0, p + 1 : 0;
	return str;
}

int strcmp(char* s1, char* s2)
{
	for (int i = 0; ; i++)
	{
		if (s1[i] != s2[i])
			return s1[i] < s2[i] ? -1 : 1;
		else if (s1[i] == '\0')
			return 0;
	}
}

void substr(int i, char *src, char *dest){ //substring exclusive
	memcpy(dest,src,i);
	dest[i] = '\0';
}

void substr(int s, int e, char *src, char *dest){ //substring exclusive range (end is exclusive, beginning is inclusive)
	memcpy(dest,&src[s],e-s);
	dest[e-s] = '\0';
}

//extern "C"
char* itoa(unsigned long num, char* str, int base)
{
	if((uint32_t)str == 0){
		str = (char*)0xC0000000;
	}

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

char* itoa(unsigned int num, char* str, int base)
{
	int i = 0;

	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	while (num != 0)
	{
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	str[i] = '\0';

	reverse(str, i);

	return str;
}

void strcpy(char* dest, const char* src)
{
	for(int i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
}