#include <memory.h>
#include <stdtype.h>

#include <task.h>

uint64_t currentPID = 0;

process_t* currentProc;
process_t* kernelProc;

extern void kmain();

void InitProcess(){
	currentProc->executed = true;
	asm volatile("mov %%eax, %%esp": :"a"(currentProc->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
	asm volatile("iret");
}

void KillProcess(){

}

void InitMultitasking(){
	kernelProc = CreateProcess("Lemon Kernel", (uint32_t)kmain);
	kernelProc->next = kernelProc;
	kernelProc->prev = kernelProc;
	InitProcess();
}

process_t* CreateProcess(char* name, uint32_t entry){
	process_t* proc = (process_t*)malloc(sizeof(process_t));
	proc->pid = currentPID;
	proc->name = name;
	proc->eip = entry;
	proc->esp = (uint32_t)malloc(0x1000);
	proc->state = PROCESS_AWAKE;
	proc->executed = false;
	asm volatile("mov %%cr3, %%eax":"=a"(proc->cr3));
	uint32_t *stack = (uint32_t*)(proc->esp + 4096);
	proc->stack = proc->esp;

	*--stack = 0x202; // eflags
	*--stack = 0x8; // cs
	*--stack = entry; // eip
	*--stack = 0; // eax
	*--stack = 0; // ebx
	*--stack = 0; // ecx;
	*--stack = 0; //edx
	*--stack = 0; //esi
	*--stack = 0; //edi
	*--stack = proc->esp + 4096; //ebp
	*--stack = 0x10; // ds
	*--stack = 0x10; // fs
	*--stack = 0x10; // es
	*--stack = 0x10; // gs

	proc->esp = (uint32_t)stack;
	return proc;
}

process_t* GetProcess(uint64_t pid){
	process_t* proc = kernelProc;
	do{
			if(proc->pid == pid) return proc;
	}while(proc != kernelProc);

	return nullptr;
}

uint64_t AddProcess(process_t *proc){
	proc->next = currentProc->next;
	proc->next->prev = proc;
	proc->prev = currentProc;
	currentProc->next = proc;
	return proc->pid;
}
