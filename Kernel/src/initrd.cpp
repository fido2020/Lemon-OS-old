#include <initrd.h>

#include <memory.h>

lemoninitfs_header_t initrd_header;
lemoninitfs_node_t* nodes;

uint32_t initrd_address;

void initrd_init(uint32_t address, uint32_t size) {
	uint32_t virtual_address = kernel_pages_allocate(size / PAGE_SIZE + 1);
	map_page(address, virtual_address, size / PAGE_SIZE + 1);
	initrd_address = virtual_address;
	initrd_header = *(lemoninitfs_header_t*)address;
	nodes = (lemoninitfs_node_t*)(address + sizeof(lemoninitfs_header_t));
}

lemoninitfs_node_t* initrd_list() {
	return nodes;
}

lemoninitfs_header_t* initrd_get_header() {
	return &initrd_header;
}

uint32_t initrd_read(int node) {
	return nodes[node].offset+initrd_address;
}

uint8_t* initrd_read(lemoninitfs_node_t node, uint8_t* buffer) {
	memcpy((void*)buffer, (void*)(initrd_address + node.offset), node.size);
	return buffer;
}

uint32_t initrd_read(char* filename) {
	for (uint32_t i = 0; i < initrd_header.num_files; i++) {
		if (nodes[i].filename == filename)
			return nodes[i].offset+initrd_address;
	}
	return 0;
}

lemoninitfs_node_t initrd_get_node(char* filename) {
	for (uint32_t i = 0; i < initrd_header.num_files; i++) {
		if (nodes[i].filename == filename)
			return nodes[i];
	}
	return nodes[0];
}