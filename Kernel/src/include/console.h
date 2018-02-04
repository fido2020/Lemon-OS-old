#pragma once

#include <stdint.h>
#include <video.h>

namespace console {
	void initialize(video_mode_t v);
	void putc(char c, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
	void puts(char* str, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
}