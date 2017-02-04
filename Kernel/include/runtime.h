#pragma once

#include <stdtype.h>

void *operator new(long unsigned int size);
void *operator new[](long unsigned int size);

void operator delete(void *p);
void operator delete[](void *p);