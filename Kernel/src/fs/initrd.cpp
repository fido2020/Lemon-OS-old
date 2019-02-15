#include <initrd.h>

#include <memory.h>
#include <filesystem.h>
#include <string.h>

lemoninitfs_header_t initrd_header;
lemoninitfs_node_t* nodes;

uint32_t initrd_address;

fs_node_t root;
fs_dirent_t root_dirent;

fs_node_t dev;
fs_dirent_t dev_dirent;
fs_node_t null;

fs_node_t* initrd_nodes;

fs_dirent_t dirent;

fs_dirent_t* read_dir(fs_node_t* node, uint32_t index);
fs_node_t* find_dir(fs_node_t* node, char* name);

uint32_t read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t read_null(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer);

void initrd_init(uint32_t address, uint32_t size) {
	uint32_t virtual_address = kernel_pages_allocate(size / PAGE_SIZE + 1);
	map_page(address, virtual_address, size / PAGE_SIZE + 1);
	initrd_address = virtual_address;
	initrd_header = *(lemoninitfs_header_t*)address;
	nodes = (lemoninitfs_node_t*)(virtual_address + sizeof(lemoninitfs_header_t));

	strcpy(root.name,"");
	root.flags = FS_NODE_DIRECTORY;
	root.inode = 0;

	root.read_dir = read_dir;
	root.find_dir = find_dir;
	root_dirent.name[0] = 0;

	strcpy(dev.name,"dev");
	dev.flags = FS_NODE_DIRECTORY;
	dev.inode = 0;

	dev.read_dir = read_dir;
	dev.find_dir = find_dir;
	strcpy(dev_dirent.name, "dev");

	strcpy(null.name,"null");
	null.flags = FS_NODE_FILE;
	null.inode = 0;

	null.read = read_null;

	initrd_nodes = (fs_node_t*)malloc(sizeof(fs_node_t)*initrd_header.num_files);

	for(int i = 0; i < initrd_header.num_files; i++){
		fs_node_t* node = &(initrd_nodes[i]);
		strcpy(node->name,nodes[i].filename);
		node->inode = i;
		node->flags = FS_NODE_FILE;
		node->read = read;
		node->write = write;
		node->open = open;
		node->close = close;
		node->size = nodes[i].size;
	}
}

uint32_t read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer){
    lemoninitfs_node_t inode = nodes[node->inode];

	if(offset > inode.size || offset + size > inode.size) return 0;
	if(node->inode >= initrd_header.num_files) return 0;

	memcpy(buffer, (void*)(inode.offset + initrd_address + offset), size);
	return size;
}

uint32_t read_null(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer){
	memset(buffer, -1, size);
	return size;
}

uint32_t write(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer){
    return 0; // It's a ramdisk.
}

void open(fs_node_t* node, uint32_t flags){
    
}

void close(fs_node_t* node){
    
}

fs_dirent_t* read_dir(fs_node_t* node, uint32_t index){
    if(node == &root && index == 0){
		return &dev_dirent;
	}

	strcpy(dirent.name,nodes[index-1].filename);


	return NULL;
}

fs_node_t* find_dir(fs_node_t* node, char* name){
    
	return NULL;
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