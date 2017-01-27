#include <stdbool.h>
#include <conioc.h>
#include <stdtype.h>
#define panic cpanic

typedef struct MemoryFrame{
	uint32_t num;
	uint32_t set;
	uint32_t pos;
} MemoryFrame;

typedef struct FrameSet{
	MemoryFrame start;
	uint32_t len;
} FrameSet;

uint8_t heap_space[0x100000]; //1MiB of heap space
uint8_t frames[8192];       //16byte Frames, 1 bit each. 1 = used, 0 = free
MemoryFrame frbuf;
FrameSet frsbuf;

void InitMemoryHeap(){
	for(int i = 0; i < 8192; i++){
		frames[i] = 0;
	}
}

MemoryFrame GetFirstAvailableFrame(){
	for(uint16_t i = 0; i < 8192; i++){
		for(uint16_t j = 0; j < 8; j++){
			if(!(frames[i] >> j & 1)){
				frbuf.num = i*8+j;
				frbuf.set = i;
				frbuf.pos = j;
				return frbuf;
			}
		}
	}
	panic("NO_HEAP_SPACE","The heap ran out of space in memory.",true);
}

FrameSet GetFirstAvailableFrameSet(uint32_t len){ //len is the amount of 16byte frames, not bytes.
	uint32_t numFrames = 0;
	for(int i = 0; i < 0x2000; i++){
		for(int j = 0; j < 8; j++){
			if(!(frames[i] >> j & 1)){
				if(numFrames == 0){
					frbuf.num = i*8+j;
					frbuf.set = i;
					frbuf.pos = j;
				}
				numFrames++;
			}else{
				numFrames = 0;
			}
			if(numFrames == len){
				frsbuf.start = frbuf;
				frsbuf.len = numFrames;
				return frsbuf;
			}
		}
	}
	panic("NO_VAR_SPACE","The memory heap has no frames to fit a variable.",true);
}

MemoryFrame GetFrame(uint32_t i){
	MemoryFrame f;
	f.set = i/8;
	f.pos = i%8;
	f.num = i;
	return f;
}

FrameSet fsalloc(uint32_t len){
	GetFirstAvailableFrameSet(len);
	MemoryFrame buf;
	for(int i = 0; i < frsbuf.len; i++){
		buf = GetFrame(i+frsbuf.start.num);
		frames[buf.set] |= 1 << buf.pos;
	}
	FrameSet fs;
	fs.start = GetFrame(frsbuf.start.num);
	fs.len = len;
	return fs;
}

void *malloc(uint32_t len){
	frsbuf = fsalloc(len/0x10+1);
	return &heap_space[frsbuf.start.num*0x10];
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
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}