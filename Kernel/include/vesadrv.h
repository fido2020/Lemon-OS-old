//
//	VESA VBE Driver
//	Thanks to Michael Petch on Stack Overflow:
//	http://stackoverflow.com/users/3857942/michael-petch
//
#pragma once

#include <stdtype.h>

struct vesa_mode_info_t * EnterGraphicsMode();

extern "C"
struct vesa_mode_info_t * do_vbe (const uint8_t video_mode);

struct vesa_mode_info_t {
    uint16_t attributes;
	uint8_t winA,winB;
	uint16_t granularity;
	uint16_t winsize;
	uint16_t segmentA, segmentB;
    uint16_t realFctPtr_offset;
    uint16_t realFctPtr_segment;

    uint16_t pitch;

    uint16_t width, height;
	uint8_t Wchar, Ychar, planes, bpp, banks;
	uint8_t memory_model, bank_size, image_pages;
	uint8_t reserved0;
	
	uint8_t red_mask, red_position;
	uint8_t green_mask, green_position;
	uint8_t blue_mask, blue_position;
	uint8_t rsv_mask, rsv_position;
	uint8_t directcolor_attributes;
	 
	uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
	uint32_t reserved1;
	uint16_t reserved2;
} __attribute__((packed));
