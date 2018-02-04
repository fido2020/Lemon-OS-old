#pragma once

#include <stdint.h>

typedef struct {
	uint32_t flags;
	uint32_t memoryLo;
	uint32_t memoryHi;
	uint32_t bootDevice;
	uint32_t cmdline;
	uint32_t modsCount;
	uint32_t modsAddr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmapLength;
	uint32_t mmapAddr;
	uint32_t drivesLength;
	uint32_t drivesAddr;
	uint32_t configTable;
	uint32_t bootloaderName;
	uint32_t apmTable;

	uint32_t vbeControlInfo;
	uint32_t vbeModeInfo;
	uint16_t vbeMode;
	uint16_t vbeInterfaceSeg;
	uint16_t vbeInterfaceOff;
	uint16_t vbeInterfaceLen;

	uint64_t framebufferAddr;
	uint32_t framebufferPitch;
	uint32_t framebufferWidth;
	uint32_t framebufferHeight;
	uint8_t framebufferBpp;
	uint8_t framebufferType;
}__attribute__ ((packed)) multiboot_info_t;

typedef struct{
    uint32_t width; // Resolution width
    uint32_t height; // Resolution height
    uint16_t bpp; // Resolution depth/bits per pixel

    uint32_t pitch; // Video mode pitch
    uint32_t type; // 1=Graphical 0=Text

    uint32_t address; // Video memory address
} video_mode_t;

typedef struct{
    // CPUID info
} cpu_info_t;

typedef struct{
    uint32_t total_memory_megabytes;
} memory_info_t;

typedef struct { //
    //bool mutliboot; // Is multiboot??
    multiboot_info_t* multiboot_info; // Multiboot header ptr
    video_mode_t video_mode_info; // Video mode info
    cpu_info_t cpu_info; // CPU Info
    memory_info_t memory_info; // Memory Info
} hardware_info_t;