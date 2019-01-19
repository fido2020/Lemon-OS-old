#pragma once

#include <stdint.h>
#include <paging.h>

#define LEMONINITFS_FILE        0x01
#define LEMONINITFS_DIRECTORY   0x02
#define LEMONINITFS_SYMLINK     0x03
#define LEMONINITFS_MOUNTPOINT  0x04

typedef struct {
	uint32_t num_files; // Amount of files present
	char versionstring[16]; // Version string
} __attribute__((packed)) lemoninitfs_header_t;

typedef struct {
	uint16_t magic; // Check for corruption (should be 0xBEEF)
	char filename[32]; // Filename
	uint32_t offset; // Offset in file
	uint32_t size; // File Size
	uint8_t type;
} __attribute__((packed)) lemoninitfs_node_t ;

void initrd_init(uint32_t address, uint32_t size);

lemoninitfs_node_t* initrd_list();
lemoninitfs_header_t* initrd_get_header();
uint32_t initrd_read(int node);
uint32_t initrd_read(char* filename);

lemoninitfs_node_t initrd_get_node(char* filename);
uint8_t* initrd_read(lemoninitfs_node_t node, uint8_t* buffer);