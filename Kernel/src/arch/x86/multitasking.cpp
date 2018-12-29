#include <multitasking.h>

#include <paging.h>
#include <memory.h>
#include <physallocator.h>
#include <list.h>
#include <serial.h>
#include <idt.h>
#include <string.h>
#include <system.h>

uint32_t thread_index = 0;

process_t* queue_start;
process_t* current_process;

uint32_t pid_next = 0;

uint32_t process_base;
uint32_t process_stack;
uint32_t process_entry_point;

uint32_t kernel_stack;

extern "C" void task_switch();

void idle_process(){
	for(;;);
}

void multitasking_init() {
	//interrupt_register_handler(IRQ0, scheduler_tick);
	//create_process((void*)idle_process);
}

void list_tasks(){
}

uint32_t create_process(void* entry) {

	// Create process structure
	process_t* proc = (process_t*)malloc(sizeof(process_t));

	proc->pid = pid_next++;
	proc->priority = 1;
	//proc->threads = (thread_t*)malloc(sizeof(thread_t));
	proc->thread_count = 1;
	proc->state = PROCESS_STATE_ACTIVE;
	proc->thread_count = 1;

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

	if(!queue_start){
		queue_start = proc;
		current_process = proc;
	}
	else if(queue_start->next){
		proc->next = queue_start->next;
		queue_start->next = proc;
	} else {
		queue_start->next = proc;
	}
	//current_process = proc;

	return proc->pid;
}

void add_thread(uint32_t pid, void* entry) {
	
	process_t* proc = current_process;
	//proc->threads = (thread_t*)realloc(proc->threads, sizeof(thread_t)*(proc->thread_count + 1));
	thread_t* thread = &proc->threads[proc->thread_count++];
	
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

	void* stack = (void*)pages_allocate(1);
	void* stack_physical = (void*)physalloc_alloc_block();

	map_page((uint32_t)stack_physical, (uint32_t)stack, 1);

	thread->initial_stack = stack;
	thread->registers.esp = (uint32_t)stack;
	thread->registers.ebp = (uint32_t)stack;
}

extern "C"
uint32_t read_eip();

void scheduler_tick(){

	uint32_t current_eip = read_eip();/*regs->eip;*/
	if(current_eip == 0x6969) return;

	uint32_t current_esp;
	uint32_t current_ebp;
	asm volatile ("mov %%esp, %0" : "=r" (current_esp));
	asm volatile ("mov %%ebp, %0" : "=r" (current_ebp));
	current_process->threads[0].registers.esp = current_esp;
	current_process->threads[0].registers.eip = current_eip;
	current_process->threads[0].registers.ebp = current_ebp;
	current_process = current_process->next;
	if(!current_process) current_process = queue_start;
	process_entry_point = current_process->threads[0].registers.eip;
	process_stack = current_process->threads[0].registers.esp;
	process_base = current_process->threads[0].registers.ebp;

	task_switch();
}