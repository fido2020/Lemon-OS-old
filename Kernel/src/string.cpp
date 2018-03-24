#include <stddef.h>
#include <string.h>
#include <memory.h>

string::string()
{
	length = 0;
	//char a[0];
	//data = a;
}

string::string(char c)
{
	length = 1;
	//char a[1];
	//data = a;
}

string::string(const char* c)
{
	if (c)
	{
		unsigned n = 0;
		while (c[n] != '\0') n++;
		length = n;
		//char a[n];
		//data = a;
		for (unsigned j = 0; j < n; j++)
			data[j] = c[j];
	}
	else
	{
		length = 0;
		//char a[0];
		//data = a;
	}
}

string::string(const string& s)
{
	length = s.len();
	///char a[length];
	//data = a;
	for (unsigned j = 0; j < length; j++)
		data[j] = s[j];
}



/*string::~string()
{
	delete[] data;
}*/


char* string::c_str() /*const*/ {
	return data;
}

size_t string::len() const {
	return length;
}

char  string::operator[] (unsigned j) const {
	if (j >= length) return NULL;
	return data[j];
}

char& string::operator[] (unsigned j) {
	if (j >= length) return data[0];
	return data[j];
}

string& string::operator= (const string& s)
{
	if (this == &s) return *this;

	//delete data;
	length = s.len();
	//char a[length];
	//data = a;
	for (unsigned j = 0; j < length; j++)
		data[j] = s[j];
	return *this;
}

string& string::operator+= (const string& s) {
	unsigned len = length + s.len();
	//char a[len];
	//char* str = a;

	//for (unsigned j = 0; j < length; j++)
		//str[j] = data[j];

	for (unsigned i = 0; i < s.len(); i++)
		//str[length + i] = s[i];
		data[length + i] = s[i];

	length = len;
	// delete previous data variable when allocation implemented
	//data = str;
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
int strindex(char c, char *str){
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

extern "C"
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
	for(int i = 0; dest[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
}

char* strtok(char* s, char delim) {
	static int index = 0;
	if (!s || !delim || s[index] == '\0')
		return NULL;
	char *newstr = (char *)malloc(sizeof(char) * strlen(s));
	int i = index, j = 0;

	while (s[i] != '\0') {
		if (s[i] != delim)
			newstr[j] = s[i];
		else goto It;
		i++;
		j++;
	}
It:
	newstr[i] = 0;
	index = i + 1;
	return newstr;
}