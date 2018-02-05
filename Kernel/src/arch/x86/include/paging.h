#pragma once

#include <stdint.h>
#include <stddef.h>
#include <system.h>

/*typedef struct {
	uint32_t present	: 1;	// Page present in memory
	uint32_t writable			: 1;	// Read-only if clear, readwrite if set
	uint32_t user		: 1;	// Supervisor level only if clear
	uint32_t accessed	: 1;	// Has the page been accessed since last refresh?
	uint32_t dirty		: 1;	// Has the page been written to since last refresh?
	uint32_t unused		: 7;	// Combination of unused and reserved bits
	uint32_t frame		: 20;	// Frame Address (Shifted right by 12 bits)
} page_t;*/

/*typedef struct {
	uint32_t present	: 1;
	uint32_t writable	: 1;
	uint32_t user		: 1;
	uint32_t present	: 1;
	uint32_t present	: 1;
} page_directory_entry_t;*/

#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR	1024
#define PAGE_SIZE 4096

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

using page_directory_t = pd_entry_t[TABLES_PER_DIR];
using page_table_t = page_t[PAGES_PER_TABLE];

void paging_initialize();

void enable_paging();
void switch_page_directory(uint32_t* dir);

void page_fault_handler(regs32_t* regs);
void map_page(uint32_t phys, uint32_t virt);

static inline void set_flags(uint32_t* target, uint32_t flags) {
	*target |= flags;
}

static inline void clear_flags(uint32_t* target, uint32_t flags) {
	*target &= ~flags;
}

static inline void page_set_frame(uint32_t* p, uint32_t addr) {
	*p = (*p & ~PAGE_FRAME) | addr;
}

static inline uint32_t page_get_frame(uint32_t p) {
	return p & PAGE_FRAME;
}

static inline void pde_set_frame(uint32_t* p, uint32_t addr) {
	*p = (*p & PDE_FRAME) | addr;
}

static inline uint32_t pde_get_frame(uint32_t p) {
	return p & PDE_FRAME;
}

static inline void flush_tlb_single(uint32_t addr)
{
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}