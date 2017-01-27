#pragma once

#include <stdtype.h>

char* strcat(char *dest, const char *src);
char* strncat(char *dest, int len, const char *src);

void reverse(char *str, int length) ;

int strlen(char *str);

int indexOf(char c, char *str);

void substr(int i, char *src, char *dest);
void substr(int s, int e, char *src, char *dest);

char* itoa(int num, char* str, int base);