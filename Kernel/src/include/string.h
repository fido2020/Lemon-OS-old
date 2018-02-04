#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
char* itoa(int num, char* str = NULL, int base = 10);
#else
char* itoa(int num, char* str, int base);
#endif

#ifdef __cplusplus
extern "C"
#endif
char* strcat(char *dest, const char *src);

#ifdef __cplusplus
class string {
	char data[300]; // Short-term fix - far from optimal
	size_t length;
public:
	//Empty String
	string();

	// Single Character
	string(char c);

	// Create String from char*
	string(const char* c);

	//Copies string to new string
	string(const string& s);

	//String destructor (no memory allocation and deallocation yet)
	//~string();

	// Get C compatible string
	char* c_str() /*const*/;

	// Get length
	size_t len() const;

	// Get character at position j
	char  operator[] (unsigned j) const;
	char& operator[] (unsigned j);

	// Set value of string
	string& operator= (const string& s);

	// Append string
	string& operator+= (const string& s);

	// Concatanate strings
	friend string operator+ (const string& left, const string& right);
	friend string operator+ (const string& left, char          right);
	friend string operator+ (const string& left, const char*   right);
	friend string operator+ (char          left, const string& right);
	friend string operator+ (const char* left, const string& right);
};
#endif