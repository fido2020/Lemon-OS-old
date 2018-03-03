#include <paging.h>
#include <idt.h>
#include <memory.h>
#include <fatal.h>
#include <string.h>
#include <serial.h>
#include <system.h>

extern uint32_t kernel_end;

uint32_t max_pages = 1024 * 1023;

page_directory_t page_directory __attribute__((aligned(4096)));
page_table_t page_tables[TABLES_PER_DIR] __attribute__((aligned(4096)));

static page_t* get_page(uint32_t addr)
{
	uint32_t pdindex = addr >> 22;
	uint32_t ptindex = addr >> 12 & 0x03FF;

	page_table_t* pt = &page_tables[pdindex];
	return &(*pt)[ptindex];
}

uint32_t pages_allocate(uint32_t amount) {
	page_t* entries = get_page(0);

	uint32_t addr = 0;
	uint32_t counter = 0;

	amount += 2;

	// keep first 4M free
	for (size_t i{ 0x100 }; i < max_pages; ++i)
	{
		if (entries[i] & PAGE_PRESENT) {
			if (counter++ == 0) addr = i;
		}
		else
			counter = 0;

		if (counter == amount)
		{
			for (size_t j{ 1 }; j < amount - 1; ++j)
			{
				set_flags(&(entries[addr + j]),PAGE_PRESENT);
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

bool pages_free(uint32_t virt, uint32_t amount)
{
	// Stub
}

// Map amount pages from phys to virt
bool map_page(uint32_t phys, uint32_t virt, uint32_t amount) {
	if (!amount) {
		write_serial_string("Error Mapping Address: Page Amount is invalid or zero (map_page)");
		return 0;
	}
	
	for (uint32_t i = 0; i < amount; i++, phys += PAGE_SIZE, virt += PAGE_SIZE) {
		// Create a new Page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writeable
		page_set_frame(&page, phys);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&page_directory[pdindex], PDE_PRESENT);
		set_flags(&page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&page_directory[pdindex], (uint32_t)&page_tables[pdindex]);
		page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
		return 1;
	}
}

void unmap_page(uint32_t addr) {
	pages_free(get_page(addr));
}

void paging_initialize()
{
	for (uint32_t i = 0; i < TABLES_PER_DIR; i++) {
		for (uint32_t j = 0; j < PAGES_PER_TABLE; j++) {
			page_tables[i][j] = 0;
		}
		page_directory[i] = 0 | PDE_WRITABLE;
	}

	for (int i = 0, frame = 0x0, virt = 0x00000000; i<4096; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

		//Create a new page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writable
		page_set_frame(&page, frame);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&page_directory[pdindex], PDE_PRESENT);
		set_flags(&page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&page_directory[pdindex], (uint32_t)&page_tables[pdindex]);
		page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
	}

	for (int i = 0, frame = 0x100000, virt = 0xc0000000; i<4096; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

		//Create a new page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writable
		page_set_frame(&page, frame);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&page_directory[pdindex], PDE_PRESENT);
		set_flags(&page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&page_directory[pdindex], (uint32_t)&page_tables[pdindex]);
		page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
	}

	interrupt_register_handler(IRQ0 + 14,page_fault_handler);

	switch_page_directory((uint32_t)page_directory - KERNEL_VIRTUAL_BASE);
	//disable_pse();
}

void switch_page_directory(uint32_t dir)
{
	asm volatile("mov %0, %%cr3":: "r"(dir));
}

void enable_paging() {
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void disable_pse() {
	uint32_t cr4;
	asm volatile("mov %%cr4, %0": "=r"(cr4));
	cr4 &= ~(1 << 4);
	asm volatile("mov %0, %%cr4":: "r"(cr4));
}

void page_fault_handler(regs32_t* regs)
{
	write_serial_string("Page Fault!");

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

	strcat(msg, itoa(fault_addr, NULL, 16));
	strcat(msg, " \nPage ");
	strcat(msg, reason);

	write_serial_string(msg);

	//fatal_error(msg, "ERR_PAGE_FAULT");

	for (;;);
}