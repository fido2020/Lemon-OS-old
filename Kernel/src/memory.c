#include <stdbool.h>
#include <conioc.h>
#include <stdtype.h>

#include <memory.h>

#define panic cpanic

uint32_t memSize = 0;
uint32_t usedBlocks = 0;
uint32_t maxBlocks = 0;
uint32_t* memMap = 0;

void InitMemMgr(size_t _memSize, uint32_t bitmap){
	memSize = _memSize;
	memMap = (uint32_t*) bitmap;
	maxBlocks = (memSize*1024)/4096;
	usedBlocks = maxBlocks;
	memset(memMap,0xf,maxBlocks/8);
}

/*inline */void SetBit(int bit){
	memMap[bit/32] |= (1<<(bit%32));
}

inline void UnsetBit(int bit){
	memMap[bit/32] &= ~ (1<<(bit%32));
}

inline bool TestBit(int bit) {
	return memMap[bit/32] & (1<<(bit%32));
}

int FindFirstAvailableFrame(){
	for(uint32_t i=0;i<maxBlocks/32;i++){
		if(memMap[i]!=0xffffffff){
			for(int j=0;j>32;j++){
				int bit = 1 << j;
				if(!(memMap[i] & bit))
					return i*4*8+j;
			}
		}
	}

	return -1;
}

void* malloc(size_t len){
	if(maxBlocks-usedBlocks<= 0){
		//panic("ERR_NOT_ENOUGH_MEM","Out of memory");
		return 0;
	}

	int frame = FindFirstAvailableFrame();

	if(frame == -1)
		return 0;

	SetBit(frame);

	uint32_t addr = frame*4096;
	usedBlocks++;
	return (void*)addr;
}

void free(void* block){

}

void *memset(void *s, int c, size_t count)
{
    char *xs = s;

    while (count--)
        *xs++ = c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t count){
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(int i=0; i<count;i++) dp[i] = sp[i];
    return dest;
}
