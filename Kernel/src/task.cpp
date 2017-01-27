#include <common.h>

#include <task.h>

using namespace console;

void kmain();
void init();

static Process* currentProcess;
static Process* kernelProcess;

uint32_t currentPID = 0;

void InitMultitasking(){
	kernelProcess = CreateProc("kernel.sys", (uint32_t)kmain);
	kernelProcess->next = kernelProcess;
	kernelProcess->prev = kernelProcess;
	currentProcess = kernelProcess;
}

Process* CreateProc(char* name, uint32_t loc){
	Process *proc = (Process*)malloc(sizeof(Process));
	memset(proc,0,sizeof(Process));
	
	proc->name = name;
	proc->pid = currentPID++;
	proc->state = PROCESS_AWAKE;
	proc->regs.eax = 0;
	proc->regs.ebx = 0;
	proc->regs.ecx = 0;
	proc->regs.esi = 0;
	proc->regs.edi = 0;
	proc->regs.eflags = 0x202;
	proc->regs.eip = loc;
	proc->regs.esp = (uint32_t)malloc(4096);
	asm volatile("mov %%cr3, %%eax":"=a"(proc->regs.cr3));
}

void PrintProcessList(){
	Process *current = kernelProcess;
	print("Running processes:\n");
	do{
		print('[');print(current == currentProcess ? '*' : ' ');print(']');print(' ');
		print(itoa(current->pid,nullptr,10));
		print(' ');
		print(current->name);
		print(' ');
		switch(current->state){
			case PROCESS_AWAKE:
				print("Awake");
			case PROCESS_IDLE:
				print("Idle");
			case PROCESS_DEAD:
				print("Dead");
			default:
				print("Unknown");
		}
		print('\n');
		current = current->next;
	}while(current != kernelProcess);
}

uint32_t AddProc(Process *proc){
	proc->next = currentProcess->next;
	proc->next->prev = proc;
	proc->prev = currentProcess;
	currentProcess->next = proc;
	return proc->pid;
}
