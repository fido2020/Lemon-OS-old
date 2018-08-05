#include <multitasking.h>

#include <paging.h>
#include <memory.h>
#include <physallocator.h>
#include <list.h>

//List<thread_t*> threads;
//thread_t* current_thread;

process_t* current_process;

uint32_t pid_next = 0;

uint32_t process_stack;
uint32_t process_entry_point;

extern "C" void execute_process_asm();

uint32_t create_process(void* entry) {

	// Create process structure
	process_t* proc = (process_t*)malloc(sizeof(process_t));

	proc->pid = pid_next++;
	proc->priority = 1;
	proc->threads = (thread_t*)malloc(sizeof(thread_t));
	proc->main_thread = proc->threads;
	proc->thread_count = 1;
	proc->state = PROCESS_STATE_ACTIVE;

	// Create structure for the main thread
	thread_t* thread = proc->threads;

	thread->initial_stack = 0;
	thread->kernel_stack = 0;
	thread->parent = proc;
	thread->priority = 1;
	thread->stack_limit = (void*)((uint32_t)thread->initial_stack + 4096);
	thread->parent = proc;

	regs32_t* registers = &thread->registers;
	memset((uint8_t*)registers, 0, sizeof(regs32_t));
	registers->eip = (uint32_t)entry;
	registers->eflags = 0x200;

	// Don't use malloc to ensure 4K alignment
	void* stack = (void*)pages_allocate(1);
	void* stack_physical = (void*)physalloc_alloc_block();

	map_page((uint32_t)stack_physical, (uint32_t)stack,1);

	thread->initial_stack = stack;
	thread->registers.esp = (uint32_t)stack;
	thread->registers.ebp = (uint32_t)stack;

	current_process = proc;

	return proc->pid;
}

void execute_process() {
	process_t* proc = current_process;

	process_entry_point = proc->threads[0].registers.eip;
	process_stack = proc->threads[0].registers.esp;

	execute_process_asm();
}

void scheduler_tick() {

}