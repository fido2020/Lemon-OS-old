#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

#if _MSC_VER
#pragma pack(push,1)
#endif

#if __GNUC__
#define PACKED __attribute__((packed))
#else
#define PACKED
#endif

typedef struct {
	uint32_t num_files; // Amount of files present
	char versionstring[16]; // Version string
} PACKED lemoninitfs_header_t;

typedef struct {
	uint16_t magic; // Check for corruption (should be 0xBEEF)
	char filename[32]; // Filename
	uint32_t offset; // Offset in file
	uint32_t size; // File Size
} PACKED lemoninitfs_node_t;
#if _MSC_VER
#pragma pack(pop)
#endif

// Usage ramdiskwriter <output> <inputfilenameonramdisk> <inputfilepathonsystem> ...
int main(int argc, char** argv) {
	lemoninitfs_header_t header;
	lemoninitfs_node_t nodes[128];

	memset(nodes, 0, sizeof(lemoninitfs_node_t)*128);

	uint32_t offset = sizeof(lemoninitfs_header_t)+sizeof(lemoninitfs_node_t)*128;

	int num_nodes = (argc - 2) / 2;
	header.num_files = num_nodes;

	for (int i = 0; i < num_nodes; i++) {
		nodes[i].magic = 0xBEEF;
		nodes[i].offset = offset;

		FILE* input = fopen(argv[(i * 2) + 2], "r");
		strcpy(nodes[i].filename, argv[(i * 2) + 3]);

		fseek(input, 0, SEEK_END);
		nodes[i].size = ftell(input);
		fclose(input);
		offset += nodes[i].size;
	}

	FILE* image = fopen(argv[1],"w");
	
	strcpy(header.versionstring,"lemoninitfs");

	fwrite(&header, sizeof(lemoninitfs_header_t), 1, image);
	fwrite(nodes, sizeof(lemoninitfs_node_t), 128, image);

	for (int i = 0; i < num_nodes; i++) {
		unsigned char* a = (unsigned char*)malloc(nodes[i].size);
		FILE* input = fopen(argv[(i * 2) + 2], "r");
		fread(a, 1, nodes[i].size, input);
		fwrite(a, 1, nodes[i].size, image);
		fclose(input);
	}

	fclose(image);
}