#include <paging.h>
#include <idt.h>
#include <memory.h>
#include <fatal.h>
#include <string.h>
#include <serial.h>
#include <system.h>
#include <video.h>
#include <physallocator.h>

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

	page_table_t* pt = &(current_page_tables[pdindex]);
	return &(*pt)[ptindex];
}

uint32_t pages_allocate(uint32_t amount, page_directory_ptr_t dir) {
	uint32_t page_dir_index = 0;
	uint32_t address = 0;
	uint32_t offset = 0;
	uint32_t counter = 0;

	amount += 2;

	for(uint32_t i = page_dir_index; i < PAGE_DIRECTORY_INDEX(0xC0000000); i++){
		for(uint32_t j = 0; j < PAGES_PER_TABLE; j++){
			if(dir.page_tables[i][j] & PAGE_PRESENT){
				offset = j;
				page_dir_index = i;
				counter = 0;
				continue;
			}

			counter++;

			if(counter == amount){
				address = (page_dir_index * PAGES_PER_TABLE + offset) * PAGE_SIZE + PAGE_SIZE;
				while(counter){
					set_flags(&(dir.page_tables[page_dir_index][offset]), PAGE_WRITABLE | PAGE_PRESENT);
					set_flags(&((*dir.page_directory)[page_dir_index]),PDE_PRESENT | PDE_WRITABLE);
					pde_set_frame(&((*dir.page_directory)[page_dir_index]),(uint32_t)&(dir.page_tables[page_dir_index]) - 0xC0000000);
					offset++;
					counter--;

					if(offset >= PAGES_PER_TABLE){
						page_dir_index++;
						offset = 0;
					} 
				}

				return address;
			}
		}
	}

	write_serial_string("No more pages availiable!\n");
	return 0;
}

uint32_t kernel_pages_allocate(uint32_t amount) { // Used for kernel heap

	uint32_t page_dir_index = PAGE_DIRECTORY_INDEX(0xC0000000);
	uint32_t address = 0;
	uint32_t offset = 0;
	uint32_t counter = 0;

	amount += 1;

	for(uint32_t i = page_dir_index; i < TABLES_PER_DIR; i++){
		for(uint32_t j = 0; j < PAGES_PER_TABLE; j++){
			if(kernel_page_tables[i][j] & PAGE_PRESENT){
				offset = j;
				page_dir_index = i;
				counter = 0;
				continue;
			}

			counter++;

			if(counter >= amount){
				address = (page_dir_index * PAGES_PER_TABLE + offset) * PAGE_SIZE + PAGE_SIZE;
				while(counter){
					set_flags(&(kernel_page_tables[page_dir_index][offset]), PAGE_WRITABLE | PAGE_PRESENT);
					set_flags(&(kernel_page_directory[page_dir_index]),PDE_PRESENT | PDE_WRITABLE);
					pde_set_frame(&(kernel_page_directory[page_dir_index]),(uint32_t)&(kernel_page_tables[page_dir_index]) - 0xC0000000);
					offset++;
					counter--;

					if(offset >= PAGES_PER_TABLE){
						page_dir_index++;
						offset = 0;
					} 
				}

				return address;
			}
		}
	}
	
	write_serial_string("No more pages for kernel heap availiable!\n");
	fatal_error("The kernel has run out of virtual memory for its heap", "KERNEL_HEAP_FULL");
	for(;;);
	//return 0;
}


uint32_t addr_virt_to_phys(uint32_t addr) {
	//return ((uint32_t*)(*current_page_directory[addr >> 22] & ~0xfff))[addr << 10 >> 10 >> 12];
	uint32_t phys_addr;

	uint32_t pd_index = PAGE_DIRECTORY_INDEX(addr);
	uint32_t pt_index = PAGE_TABLE_INDEX(addr);
	
	phys_addr = (page_get_frame(current_page_tables[pd_index][pt_index])) << 12;
	return phys_addr;
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

		/*write_serial_string("\r\nMapping 0x");
		write_serial_string(itoa(virt,(char*)0xC0000000,16));
		write_serial_string(" to 0x");
		write_serial_string(itoa(phys,(char*)0xC0000000,16));*/

		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);

		// Get the page from the page table
		page_t* page = get_page(virt);
		set_flags(page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writeable
		page_set_frame(page, phys); // Point it to the physical address

		// Add it to the page table and directory
		set_flags(&((*current_page_directory)[pdindex]), PDE_PRESENT | PDE_WRITABLE);
		pde_set_frame(&((*current_page_directory)[pdindex]), (uint32_t)&(current_page_tables[pdindex]) - 0xC0000000);
	}
	return 1;
}

// Map amount pages from phys to virt
bool map_page(uint32_t phys, uint32_t virt, uint32_t amount, page_directory_ptr_t pdir) {
	if (!amount) {
		write_serial_string("Error Mapping Address: Page Amount is invalid or zero (map_page)");
		return 0;
	}

	for (uint32_t i = 0; i < amount; i++, phys += PAGE_SIZE, virt += PAGE_SIZE) {
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);

		page_table_t* old_page_tables = current_page_tables;

		// Get the page from the page table
		current_page_tables = pdir.page_tables;
		page_t* page = get_page(virt);
		current_page_tables = old_page_tables;
		set_flags(page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writeable
		page_set_frame(page, phys); // Point it to the physical address

		// Add it to the page table and directory
		set_flags(&((*pdir.page_directory)[pdindex]), PDE_PRESENT | PDE_WRITABLE);
		pde_set_frame(&((*pdir.page_directory)[pdindex]), (uint32_t)addr_virt_to_phys((uint32_t)&(pdir.page_tables[pdindex])));
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
	interrupt_register_handler(14,page_fault_handler);

	current_page_directory = &kernel_page_directory;
	current_page_tables = kernel_page_tables;

	write_serial_string("\r\n0x");
	write_serial_string(itoa((uint32_t)(&kernel_end),0,16));
	write_serial_string("\r\n");

	for (uint32_t i = 0; i < TABLES_PER_DIR; i++) {
		for (uint32_t j = 0; j < PAGES_PER_TABLE; j++) {
			kernel_page_tables[i][j] = 0;
		}
		kernel_page_directory[i] = 0 | PDE_WRITABLE;
	}

	for (int i = 0, frame = 0x0, virt = 0; i<6144; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

		//Create a new page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT | PAGE_WRITABLE); // Make it present and writable
		page_set_frame(&page, frame);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&kernel_page_directory[pdindex], PDE_PRESENT);
		set_flags(&kernel_page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&(kernel_page_directory[pdindex]), ((uint32_t)&(kernel_page_tables[pdindex])) - 0xC0000000);
		kernel_page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
	}

	for (int i = 0, frame = 0x0, virt = 0xC0000000; i<6144; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

		//Create a new page
		page_t page = 0;
		set_flags(&page, PAGE_PRESENT); // Make it present and writable
		set_flags(&page, PAGE_WRITABLE); // Make it present and writable
		page_set_frame(&page, frame);

		// Add it to the page table and directory
		uint32_t pdindex = PAGE_DIRECTORY_INDEX(virt);
		set_flags(&kernel_page_directory[pdindex], PDE_PRESENT);
		set_flags(&kernel_page_directory[pdindex], PDE_WRITABLE);
		pde_set_frame(&(kernel_page_directory[pdindex]), ((uint32_t)&(kernel_page_tables[pdindex])) - 0xC0000000);
		kernel_page_tables[pdindex][PAGE_TABLE_INDEX(virt)] = page;
	}

	switch_page_directory((uint32_t)kernel_page_directory - 0xC0000000);
}

void map_kernel() {
	for (int i = 0, frame = 0x0, virt = 0xC0000000; i<8192; i++, frame += PAGE_SIZE, virt += PAGE_SIZE) {

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
}

// Switch Page Directory
void switch_page_directory(uint32_t dir)
{
	asm volatile("mov %0, %%cr3":: "r"(dir));
}

// Sets the current page directory but does not switch
void set_current_page_directory(page_directory_ptr_t dir) {
	current_page_directory = dir.page_directory;
	current_page_tables = dir.page_tables;
}

// Sets the current page directory to that of the kernel but does not switch
void set_current_page_directory_kernel() {
	current_page_directory = &kernel_page_directory;
	current_page_tables = kernel_page_tables;
}

page_directory_ptr_t new_address_space() {
	asm("cli");
	page_directory_ptr_t ptr;

	ptr.page_directory = (page_directory_t*)kernel_pages_allocate(1); // Allocate space for the page directory
	ptr.page_directory_phys = physalloc_alloc_block();

	map_page((uint32_t)ptr.page_directory_phys, (uint32_t)ptr.page_directory, 1); // Map page directory into kernel space

	for(uint32_t i = PAGE_DIRECTORY_INDEX(0xC0000000); i < TABLES_PER_DIR; i++){
		set_flags(&((*ptr.page_directory)[i]), PDE_PRESENT | PDE_WRITABLE); // Map everything above the 3GB mark (kernel space)
		pde_set_frame(&((*ptr.page_directory)[i]), (uint32_t)&(kernel_page_tables[i]) - 0xC0000000);
	}

	ptr.page_tables = (page_table_t*)kernel_pages_allocate(sizeof(page_table_t) * PAGE_DIRECTORY_INDEX(0xC0000000) / PAGE_SIZE + PAGE_SIZE); // Don't allocate space for page tables above 3 GB in memory

	for(uint32_t i = 0; i < PAGE_DIRECTORY_INDEX(0xC0000000); i++){
		uint32_t phys = physalloc_alloc_block();
		map_page(phys,(uint32_t)&(ptr.page_tables[i]), 1); // 1 page table takes up 4096 bytes (1 page) in memory
	}

	for(uint32_t i = 0; i < PAGE_DIRECTORY_INDEX(0xC0000000); i++){
		set_flags(&((*ptr.page_directory)[i]), PDE_PRESENT | PDE_WRITABLE | PDE_USER);
		pde_set_frame(&((*ptr.page_directory)[i]), addr_virt_to_phys((uint32_t)&(ptr.page_tables[i])));
	}

	memset((uint8_t*)ptr.page_tables, 0, sizeof(page_table_t) * TABLES_PER_DIR);
	asm("sti");
	return ptr;
}

char addrbuf[16];
void page_fault_handler(regs32_t* regs)
{
	write_serial_string("Page Fault!\r\n");

	uint32_t fault_addr;
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	int present = !(regs->err_code & 0x1); // Page not present
	int rw = regs->err_code & 0x2;           // Attempted write to read only page
	int us = regs->err_code & 0x4;           // Processor was in user-mode and tried to access kernel page
	int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry
	int id = regs->err_code & 0x10;          // Caused by an instruction fetch

	char* msg = "Page fault ";
	char* reason;
	if (present)
		reason = "page not present";
	if (rw)
		reason = "read-only ";
	if (us)
		reason = "user-mode ";
	if (reserved)
		reason = "reserved ";
	if (id)
		reason = "instruction fetch";

	
	write_serial_string(reason); // Print fault to serial

	strcat(msg, reason);

	write_serial_string(msg);
	write_serial_string("\r\nEIP: 0x");

	write_serial_string(itoa(regs->eip,(char*)0xC0000000,16));
	write_serial_string("\r\n");
	
	itoa(fault_addr,addrbuf,16);

	write_serial_string("\r\nFault address: ");
	write_serial_string(addrbuf);

	fatal_error(msg, addrbuf);

	for (;;);
}