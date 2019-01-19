#pragma once

#include <stdint.h>
#include <stddef.h>
#include <system.h>


#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR	1024
#define PAGE_SIZE 4096
#define PAGE_SHIFT 12

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

typedef uint32_t pd_entry_t;
typedef uint32_t page_t;



enum PAGE_FLAGS {

	PAGE_PRESENT = 1,
	PAGE_WRITABLE = 2,
	PAGE_USER = 4,
	PAGE_WRITETHOUGH = 8,
	PAGE_CACHE_DISABLED = 0x10,
	PAGE_ACCESSED = 0x20,
	PAGE_DIRTY = 0x40,
	PAGE_PAT = 0x80,
	PAGE_CPU_GLOBAL = 0x100,
	PAGE_LV4_GLOBAL = 0x200,
	PAGE_FRAME = 0x7FFFF000
};

enum PAGE_DIRECTORY_ENTRY_FLAGS {

	PDE_PRESENT = 1,
	PDE_WRITABLE = 2,
	PDE_USER = 4,
	PDE_WRITETHROUGH = 8,
	PDE_CACHE_DISABLED = 0x10,
	PDE_ACCESSED = 0x20,
	PDE_DIRTY = 0x40,
	PDE_PAGE_SIZE = 0x80, // 0 = 4 KB, 1 = 4 MB
	PDE_CPU_GLOBAL = 0x100,
	PDE_LV4_GLOBAL = 0x200,
	PDE_FRAME = 0x7FFFF000
};

static inline void page_set_frame(uint32_t* p, uint32_t addr) {
	*p = (*p & ~PAGE_FRAME) | addr;
}

static inline uint32_t page_get_frame(uint32_t p) {
	return (p & PAGE_FRAME) >> 12;
}

static inline void pde_set_frame(uint32_t* p, uint32_t addr) {
	*p = (*p & ~PDE_FRAME) | addr;
}

static inline uint32_t pde_get_frame(uint32_t p) {
	return (p & PDE_FRAME) >> 12;
}


using page_directory_t = pd_entry_t[TABLES_PER_DIR];
using page_table_t = page_t[PAGES_PER_TABLE];
using page_tables_t = page_table_t[TABLES_PER_DIR];

typedef struct {
	page_directory_t* page_directory;
	page_table_t* page_tables;
} __attribute__((packed)) page_directory_ptr_t;

void paging_initialize();

extern "C" void enable_paging();

void switch_page_directory(uint32_t dir);

void set_current_page_directory(page_directory_ptr_t dir);
void set_current_page_directory_kernel();

void map_kernel();

void page_fault_handler(regs32_t* regs);
bool map_page(uint32_t phys, uint32_t virt, uint32_t amount);
void unmap_page(uint32_t addr);

uint32_t pages_allocate(uint32_t amount);
uint32_t kernel_pages_allocate(uint32_t amount);
void pages_free(uint32_t virt, uint32_t amount);

bool mark_pages_reserved(uint32_t phys, uint32_t virt, uint32_t amount);

static inline void set_flags(uint32_t* target, uint32_t flags) {
	*target |= flags;
}

static inline void clear_flags(uint32_t* target, uint32_t flags) {
	*target &= ~flags;
}

static inline void flush_tlb_entry(uint32_t addr)
{
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

page_directory_ptr_t new_address_space();

uint32_t addr_virt_to_phys(uint32_t addr);