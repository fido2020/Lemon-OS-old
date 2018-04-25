#include <paging.h>
#include <idt.h>
#include <memory.h>
#include <fatal.h>
#include <string.h>
#include <serial.h>
#include <system.h>
extern uint32_t kernel_end;

uint32_t max_pages = 1024 * 1023;

page_directory_t kernel_page_directory __attribute__((aligned(4096)));
page_table_t kernel_page_tables[TABLES_PER_DIR] __attribute__((aligned(4096)));

page_directory_t* current_page_directory;
page_table_t* current_page_tables;

static page_t* get_page(uint32_t addr)
{
	uint32_t pdindex = PAGE_DIRECTORY_INDEX(addr);
	uint32_t ptindex = PAGE_TABLE_INDEX(addr);

	set_flags(&(*current_page_directory)[pdindex], PDE_PRESENT | PDE_WRITABLE);
	pde_set_frame(&(*current_page_directory)[pdindex], (uint32_t)&current_page_tables[pdindex]);

	page_table_t* pt = &(current_page_tables[pdindex]);
	return &(*pt)[ptindex];
}

uint32_t pages_allocate(uint32_t amount) {
	page_t* entries = get_page(0);

	uint32_t addr = 0;
	uint32_t counter = 0;

	amount += 2;

	// keep first 28M free
	for (size_t i = 4096; i < max_pages; ++i)
	{
		if (entries[i] & PAGE_PRESENT) {
			if (counter++ == 0) addr = i;
		}
		else
			counter = 0;

		if (counter == amount)
		{
			for (size_t j = 1; j < amount - 1; ++j)
			{
				set_flags(&(entries[addr + j]),PAGE_PRESENT | PAGE_WRITABLE);
			}
			return addr * PAGE_SIZE + PAGE_SIZE;
		}
	}
	write_serial_string("No more pages availiable!\n");
	return 0;
}

void pages_free(page_t* p) {
	uint32_t frame;
	if ((frame = page_get_frame(*p))) {
		clear_flags(p, PAGE_PRESENT);
		flush_tlb_entry((uint32_t)p);
	}
}

void pages_free(uint32_t virt, uint32_t amount)
{
	for (uint32_t i = 0; i < amount; i++) {
		page_t* pg = get_page(virt + i*PAGE_SIZE);
		clear_flags(pg, PAGE_PRESENT);
	}
}

// Map amount pages from phys to virt
bool map_page(uint32_t phys, uint32_t virt, uint32_t amount) {
	if (!amount) {
		write_serial_string("Error Mapping Address: Page Amount is invalid or zero (map_page)");
		return 0;
	}

	for (uint32_t i = 0; i < amount; i++, phys += PAGE_SIZE, virt += PAGE_SIZE) {

		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);

		// Get the page from the page table
		page_t* page = get_page(virt); // &(current_page_tables[pdindex][PAGE_TABLE_INDEX(virt)]);
		set_flags(page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writeable
		page_set_frame(page, phys); // Point it to the physical address

		// Add it to the page table and directory
		set_flags(&((*current_page_directory)[pdindex]), PDE_PRESENT | PDE_WRITABLE);
		pde_set_frame(&((*current_page_directory)[pdindex]), (uint32_t)&(current_page_tables[pdindex]));
	}
	return 1;
}

bool mark_pages_reserved(uint32_t phys, uint32_t virt, uint32_t amount) {
	if (!amount) {
		write_serial_string("Error Mapping Address: Page Amount is invalid or zero (mark_pages_reserved)");
		return 0;
	}

	for (uint32_t i = 0; i < amount; i++, phys += PAGE_SIZE, virt += PAGE_SIZE) {
		// Create a new Page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present
		page_set_frame(&page, phys);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&(*(current_page_directory)[pdindex]), PDE_PRESENT | PDE_WRITABLE);
		pde_set_frame(&(*(current_page_directory)[pdindex]), (uint32_t)&current_page_tables[pdindex]);
		current_page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
		return 1;
	}
	return 0;
}


void unmap_page(uint32_t addr) {
	pages_free(get_page(addr));
}

void paging_initialize()
{
	current_page_directory = &kernel_page_directory;
	current_page_tables = kernel_page_tables;

	for (uint32_t i = 0; i < TABLES_PER_DIR; i++) {
		for (uint32_t j = 0; j < PAGES_PER_TABLE; j++) {
			kernel_page_tables[i][j] = 0;
		}
		kernel_page_directory[i] = 0 | PDE_WRITABLE;
	}

	for (int i = 0, frame = 0x0, virt = 0x00000000; i<8192; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

		//Create a new page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writable
		page_set_frame(&page, frame);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&kernel_page_directory[pdindex], PDE_PRESENT);
		set_flags(&kernel_page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&kernel_page_directory[pdindex], (uint32_t)&kernel_page_tables[pdindex]);
		kernel_page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
	}

	/*for (int i = 0, frame = 0x100000, virt = 0xc0000000; i<4096; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

		//Create a new page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writable
		page_set_frame(&page, frame);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&kernel_page_directory[pdindex], PDE_PRESENT);
		set_flags(&kernel_page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&kernel_page_directory[pdindex], (uint32_t)&kernel_page_tables[pdindex]);
		kernel_page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
	}*/

	interrupt_register_handler(14,page_fault_handler);

	switch_page_directory((uint32_t)kernel_page_directory);
	enable_paging();
}

// Switch Page Directory
void switch_page_directory(uint32_t dir)
{
	asm volatile("mov %0, %%cr3":: "r"(dir));
}

// Enable Paging
void enable_paging() {
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

// Clone specified page directory
page_directory_ptr_t clone_page_directory(page_directory_ptr_t src) {
	page_directory_t* dir = (page_directory_t*)malloc(sizeof(page_directory_t));
	page_table_t* tables = (page_table_t*)malloc(sizeof(page_table_t)*TABLES_PER_DIR);

	page_directory_ptr_t dir_ptr;
	dir_ptr.page_directory = dir;
	dir_ptr.page_tables = tables;

	for (int i = 0; i < TABLES_PER_DIR; i++) {
		(*dir)[i] = (*src.page_directory)[i];
		for (int j = 0; j < PAGES_PER_TABLE; i++) {
			tables[i][j] = src.page_tables[i][j];
		}
	}
	return dir_ptr;
}

void page_fault_handler(regs32_t* regs)
{
	write_serial_string("Page Fault!\r\n");

	uint32_t fault_addr;
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	int present = !(regs->err_code & 0x1); // Page not present
	int rw = regs->err_code & 0x2;           // Write operation?
	int us = regs->err_code & 0x4;           // Processor was in user-mode?
	int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

	char* msg = "Page fault at address: 0x";
	char* reason;
	if (present)
		reason = "present ";
	if (rw)
		reason = "read-only ";
	if (us)
		reason = "user-mode ";
	if (reserved)
		reason = "reserved ";
	if (id)
		reason = "instruction fetch";

	
	write_serial_string(reason);

	strcat(msg, itoa(fault_addr, NULL, 16));

	write_serial_string(msg);
	write_serial_string(" ");

	fatal_error(msg, "ERR_PAGE_FAULT");

	for (;;);
}