#pragma once

#include <stdint.h>
#include <paging.h>

#define PROCESS_STATE_SUSPENDED 0
#define PROCESS_STATE_

typedef struct process_t;

typedef struct {
	uint32_t esp;
	uint32_t ebp;
	uint32_t eip;
	uint32_t edi;
	uint32_t esi;
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t flags;
} trapframe_t;

typedef struct {
	process_t* parent;
	void* initial_stack;
	void* stack_limit;
	void* kernel_stack;
	uint32_t priority;
	uint32_t state;
	trapframe_t trap_frame; 
} thread_t;

typedef struct {
	uint32_t pid;
	uint32_t priority;
	page_directory_ptr_t page_directory;
	uint32_t state;
	thread_t* threads;
} process_t;