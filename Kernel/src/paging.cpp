#include <common.h>
#include <paging.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_table[1024] __attribute__((aligned(4096)));

void InitPaging(){
	uint32_t i,j;
	page_directory[0]= 0x83;
	for(i = 1; i < (KERNEL_VIRTUAL_BASE >> 22); i++){
		page_directory[i] = 0x0;
	}
	page_directory[i] = 0x00000083;
	i++;
	for(i=i; i < 1024; i++){
		page_directory[i] = 0;
	}
	uint32_t *d = (uint32_t*)4096;
	LoadPageDirectory((uint32_t *)((uint32_t)&page_directory[0]-KERNEL_VIRTUAL_BASE));
}