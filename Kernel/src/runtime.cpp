#include <common.h>

#include <runtime.h>

void *operator new(long unsigned int size)
{
    return malloc(size);
}
 
void *operator new[](long unsigned int size)
{
    return malloc(size);
}
 
void operator delete(void *p)
{
    //free(p);
}
 
void operator delete[](void *p)
{
    //free(p);
}