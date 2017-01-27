#pragma once

#include <common.h>

#define PROCESS_AWAKE 0
#define PROCESS_IDLE 1
#define PROCESS_DEAD 2

void InitMultitasking();

typedef struct {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

typedef struct Process {
	char* name;
	uint32_t pid;
	uint8_t state;
	Registers regs;
	struct Process *next,*prev;
} Process;

void InitMultitasking();
Process* CreateProc(char* name, uint32_t loc);
void PrintProcessList();
uint32_t AddProc(Process *p);