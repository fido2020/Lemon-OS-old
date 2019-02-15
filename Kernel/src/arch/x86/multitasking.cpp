#include <multitasking.h>

#include <paging.h>
#include <memory.h>
#include <physallocator.h>
#include <list.h>
#include <serial.h>
#include <idt.h>
#include <string.h>
#include <system.h>
#include <elf.h>

uint32_t thread_index = 0;

process_t* queue_start = 0;
process_t* current_process = 0;

uint32_t pid_next = 0;

uint32_t process_base;
uint32_t process_stack;
uint32_t process_entry_point;
uint32_t process_page_directory;

uint32_t kernel_stack;

bool scheduler_lock = true;

extern "C" void task_switch();

extern "C"
void idle_proc_();

List<handle_index_t>* handles;

void multitasking_init() {
	scheduler_lock = true;
	handles = new List<handle_index_t>();
	create_process((void*)idle_proc_);
	process_entry_point = current_process->threads[0].registers.eip;
	process_stack = current_process->threads[0].registers.esp;
	process_base = current_process->threads[0].registers.ebp;
	process_page_directory = current_process->page_directory.page_directory_phys;
	asm("cli");
	scheduler_lock = false;
	task_switch();
}

process_t* get_current_proc(){ return current_process; }

void register_handle(handle_t handle){
	handle_index_t index;
	index.handle = handle;
	index.owner = current_process;
	index.owner_pid = current_process->pid;

	handles->add_back(index);
}

handle_index_t find_handle(handle_t handle){
	for(int i = 0; i < handles->get_length(); i++){
		if((*handles)[i].handle == handle) return (*handles)[i];
	}
	handle_index_t null_index;
	null_index.handle = 0;
	return null_index;
}

uint32_t create_process(void* entry) {

	bool scheduler_state = scheduler_lock;
	scheduler_lock = true;

	// Create process structure
	process_t* proc = (process_t*)malloc(sizeof(process_t));
	write_serial_string("proc addr:");
	write_serial_string(itoa((uint32_t)proc, 0, 16));
	write_serial_string("\r\n");
	memset(proc,0,sizeof(process_t));

	proc->pid = pid_next++;
	proc->priority = 1;
	proc->thread_count = 1;
	proc->state = PROCESS_STATE_ACTIVE;
	proc->thread_count = 1;

	proc->page_directory = new_address_space();

	// Create structure for the main thread
	thread_t* thread = proc->threads;

	thread->initial_stack = 0;
	thread->kernel_stack = 0;
	thread->parent = proc;
	thread->priority = 1;
	thread->stack_limit = (void*)((uint32_t)thread->initial_stack);
	thread->parent = proc;

	regs32_t* registers = &thread->registers;
	memset((uint8_t*)registers, 0, sizeof(regs32_t));
	registers->eflags = 0x200;

	void* stack = (void*)kernel_pages_allocate(4);
	for(int i = 0; i < 4; i++){
		map_page(physalloc_alloc_block(),(uint32_t)stack + PAGE_SIZE * i, 1);
	}

	thread->initial_stack = stack + 16384;
	thread->registers.esp = (uint32_t)thread->initial_stack;
	thread->registers.ebp = (uint32_t)thread->initial_stack;
	thread->registers.eip = (uint32_t)entry;

	if(!queue_start){
		queue_start = proc;
		proc->next = proc;
		current_process = proc;
	}
	else if(queue_start->next){
		proc->next = queue_start->next;
		queue_start->next = proc;
	} else {
		queue_start->next = proc;
		proc->next = queue_start;
	}

	scheduler_lock = scheduler_state;

	return proc->pid;
}

process_t* find_by_pid(uint32_t pid){
	process_t* proc = queue_start;
	uint32_t start_pid = proc->pid; // Used to check if we loop back to this process
	while(proc){
		if(proc->pid == pid) // Is it the process with the PID needed?
			return proc; // Return
		else if(proc->pid == start_pid) return NULL; // Make sure we don't enter into an infinite loop
		proc = proc->next; // Go to next process
	}
}

uint32_t create_process_elf(void* elf) {

	bool scheduler_state = scheduler_lock;
	scheduler_lock = true;

	// Create process structure
	process_t* proc = (process_t*)malloc(sizeof(process_t));
	write_serial_string("proc addr:");
	write_serial_string(itoa((uint32_t)proc, 0, 16));
	write_serial_string("\r\n");
	memset(proc,0,sizeof(process_t));

	proc->pid = pid_next++;
	proc->priority = 1;
	//proc->threads = (thread_t*)malloc(sizeof(thread_t));
	proc->thread_count = 1;
	proc->state = PROCESS_STATE_ACTIVE;
	proc->thread_count = 1;

	proc->page_directory = new_address_space();

	elf32_hdr_t elf_hdr = *(elf32_hdr_t*)elf;

	elf32_section_header_t elf_shstr = *((elf32_section_header_t*)((uint32_t)elf + elf_hdr.e_shoff + elf_hdr.e_shstrndx * elf_hdr.e_shentsize));

	asm("cli");

	if(true){
		for(int i = 0; i < elf_hdr.e_phnum; i++){
			elf32_program_header_t elf_ph = *((elf32_program_header_t*)(elf + elf_hdr.e_phoff + i * elf_hdr.e_phentsize));

			if(elf_ph.p_memsz == 0) continue;

			for(int i = 0; i < (elf_ph.p_memsz / PAGE_SIZE) + 1; i++){
				uint32_t phys = physalloc_alloc_block();
				map_page(phys,elf_ph.p_vaddr + i * PAGE_SIZE, 1, current_process->page_directory);
				map_page(phys,elf_ph.p_vaddr + i * PAGE_SIZE, 1, proc->page_directory);
			}

			if (elf_ph.p_memsz > 0){
				write_serial_string("\r\nElf section address 0x");
				write_serial_string(itoa(elf_ph.p_vaddr, (char*)0xC0000000, 16));
				memcpy((void*)elf_ph.p_vaddr,(void*)(elf + elf_ph.p_offset),elf_ph.p_memsz);

			}
		}
	}
	write_serial_string("at elf entry: ");
	write_serial_string(itoa(*((uint16_t*)elf_hdr.e_entry),0,16));
	write_serial_string("\r\nelf entry: ");
	write_serial_string(itoa(elf_hdr.e_entry,0,16));
	asm("sti");

	// Create structure for the main thread
	thread_t* thread = proc->threads;

	thread->initial_stack = 0;
	thread->kernel_stack = 0;
	thread->parent = proc;
	thread->priority = 1;
	thread->stack_limit = (void*)((uint32_t)thread->initial_stack);
	thread->parent = proc;

	regs32_t* registers = &thread->registers;
	memset((uint8_t*)registers, 0, sizeof(regs32_t));
	registers->eflags = 0x200;

	write_serial_string("\r\nEntry: ");
	write_serial_string(itoa((uint32_t)elf_hdr.e_entry,0,16));
	write_serial_string("\r\n");

	// Don't use malloc to ensure 4K alignment
	void* stack = (void*)kernel_pages_allocate(4);
	for(int i = 0; i < 4; i++){
		map_page(physalloc_alloc_block(),(uint32_t)stack + PAGE_SIZE * i, 1);
	}

	thread->initial_stack = stack + 16384;
	thread->registers.esp = (uint32_t)thread->initial_stack;
	thread->registers.ebp = (uint32_t)thread->initial_stack;
	memset((void*)(thread->initial_stack-16384),0,16384);
	thread->registers.eip = elf_hdr.e_entry;

	if(!queue_start){
		queue_start = proc;
		proc->next = proc;
		current_process = proc;
	}
	else if(queue_start->next){
		proc->next = queue_start->next;
		queue_start->next = proc;
	} else {
		queue_start->next = proc;
		proc->next = queue_start;
	}

	scheduler_lock = scheduler_state;

	return proc->pid;
}

extern "C"
uint32_t read_eip();

void scheduler_tick(){
	if(scheduler_lock) return;

	uint32_t current_esp;
	uint32_t current_ebp;
	asm volatile ("mov %%esp, %0" : "=r" (current_esp));
	asm volatile ("mov %%ebp, %0" : "=r" (current_ebp));

	uint32_t current_eip = read_eip();

	if(current_eip == 0xC0000123) {
		return;
	}

	current_process->threads[0].registers.esp = current_esp;
	current_process->threads[0].registers.eip = current_eip;
	current_process->threads[0].registers.ebp = current_ebp;

	current_process = current_process->next;

	process_entry_point = current_process->threads[0].registers.eip;
	process_stack = current_process->threads[0].registers.esp;
	process_base = current_process->threads[0].registers.ebp;
	process_page_directory = current_process->page_directory.page_directory_phys;
	task_switch();
}
