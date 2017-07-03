#include <stdint.h>
#include <memory.h>
#include <misc.h>

static uint8_t* vidmem;
static uint8_t* buffer;

static video_mode_info* modeInfo;

void gfx_init(video_mode_info* videoModeInfo){
    vidmem = (uint8_t*)videoModeInfo->addr;
    modeInfo = videoModeInfo;
    buffer = /*(uint8_t*)malloc(modeInfo->width*(modeInfo->bpp/8)*modeInfo->height)*/;
    for(int i=0;i<modeInfo->width*(modeInfo->bpp/8)*modeInfo->height;i++)
        buffer[i] = 128;
}

void putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b){
	unsigned where = x*(modeInfo->bpp/8) + y*modeInfo->pitch;
    buffer[where + 0] = b;
    buffer[where + 1] = g;
    buffer[where + 2] = r;
}

void gfx_updatebuffer(){
    unsigned int visibleBytesPerLine = modeInfo->width * modeInfo->bpp / 8;
    uint8_t *dest = vidmem;
    uint8_t *src = buffer;

    for(int y = 0; y < modeInfo->height; y++) {
        for(int x=0;x<visibleBytesPerLine;x++)
            dest[x] = src[x];
        dest += modeInfo->pitch;
        src += visibleBytesPerLine;
    }
}