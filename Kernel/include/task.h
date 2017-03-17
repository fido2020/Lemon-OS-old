#pragma once

#include <common.h>

#define PROCESS_AWAKE 0
#define PROCESS_IDLE 1
#define PROCESS_DEAD 2

void InitMultitasking();

typedef struct process_t{
	char *name;
	uint64_t pid;
	uint32_t esp;
	uint32_t stack;
	uint32_t eip;
	uint32_t cr3;
	uint32_t state;
	void (*notify)(uint32_t);
	bool executed;
	struct process_t *next, *prev;
} process_t;

void InitMultitasking();
process_t* CreateProcess(char* name, uint32_t entry);
void PrintProcessList();
uint64_t AddProcess(process_t *proc);
