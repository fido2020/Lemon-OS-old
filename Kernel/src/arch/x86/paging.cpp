#include <paging.h>
#include <idt.h>
#include <memory.h>
#include <fatal.h>
#include <string.h>
#include <serial.h>
#include <system.h>

int z;

extern uint32_t kernel_end;

uint32_t max_pages = 1024 * 1023;

page_directory_t page_directory;
page_table_t page_tables[1024];

static page_t* get_page(uint32_t addr)
{
	uint32_t pdindex = addr >> 22;
	uint32_t ptindex = addr >> 12 & 0x03FF;

	page_table_t* pt = (page_table_t*)(((pde_get_frame(page_directory[pdindex]) /* << 12*/) + KERNEL_VIRTUAL_BASE));
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
	page_t* page_entry = get_page(virt);
	for (size_t i{ 0 }; i < amount; ++i)
	{
		clear_flags(page_entry, PAGE_PRESENT);
		flush_tlb_entry(virt + i * PAGE_SIZE);
	}

	return true;
}

void map_page(uint32_t phys, uint32_t virt) {
	page_t* page_entry = get_page(virt);

	uint32_t flags = PAGE_PRESENT | PAGE_WRITABLE;
	set_flags(page_entry, flags);
	page_set_frame(page_entry, (uint32_t)phys >> 12);
}

void unmap_page(uint32_t addr) {
	pages_free(get_page(addr));
}

void paging_initialize()
{
	asm("cli");
	memset((uint8_t*)page_directory, 0, sizeof(page_directory_t));

	for (uint32_t i = 0; i < TABLES_PER_DIR; ++i)
	{
		memset((uint8_t*)(&page_tables[i]), 0, PAGES_PER_TABLE * sizeof(page_t));
		pde_set_frame(&page_directory[i], ((uint32_t)(&page_tables[i]) - KERNEL_VIRTUAL_BASE) >> 12);
		set_flags(&page_directory[i], PDE_PRESENT | PDE_WRITABLE);
	}

	page_t* page = get_page(KERNEL_VIRTUAL_BASE);

	for (uint32_t addr = 0; addr <= (((uint32_t)&kernel_end)-KERNEL_VIRTUAL_BASE) + PAGE_SIZE; addr += PAGE_SIZE, ++page)
	{
		page_set_frame(page, addr >> 12);
		set_flags(page, PAGE_PRESENT | PAGE_WRITABLE);
	}

	// Map last page directory entry to itself
	pde_set_frame(&page_directory[1023], ((uint32_t)page_directory - KERNEL_VIRTUAL_BASE) >> 12);
	set_flags(&page_directory[1023], PDE_PRESENT | PDE_WRITABLE);

	write_serial_string("!");
	
	//interrupt_register_handler(IRQ0 + 14,page_fault_handler);

	switch_page_directory((uint32_t)page_directory - KERNEL_VIRTUAL_BASE);
	disable_pse();

	asm("sti");
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