#pragma once

#include <stdint.h>
#include <paging.h>

#define PROCESS_STATE_SUSPENDED 0
#define PROCESS_STATE_ACTIVE 1
struct process;

typedef struct {
	process* parent; // Parent Process
	void* initial_stack; // Pointer to the initial stack
	void* stack_limit; // The limit of the stack
	void* kernel_stack; // Pointer to the kernel stack
	uint8_t priority; // Thread priority
	uint8_t state; // Thread state
	regs32_t registers;  // Registers
} thread_t;

typedef struct process {
	uint32_t pid; // PID
	uint8_t priority; // Process Priority
	page_directory_ptr_t page_directory; // Pointer to page directory and tables
	uint8_t state; // Process state
	//thread_t* threads; // Array of threads
	thread_t threads[1];
	uint32_t thread_count; // Amount of threads
	process* next;
} process_t;

uint32_t create_process(void* entry);
void add_thread(uint32_t pid, void * entry);
void scheduler_tick(regs32_t *r);
void multitasking_init();