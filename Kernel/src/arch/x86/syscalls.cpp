#include <syscalls.h>

#include <system.h>
#include <idt.h>

void* syscalls[] = {
	//(void*)&screen_clear_direct
};

uint32_t num_syscalls = 1;

void syscalls_init() {
	interrupt_register_handler(0x69, syscall_handler);
}

void syscall_handler(regs32_t* regs) {
	if (regs->eax >= num_syscalls)
		return;

	void* function = syscalls[regs->eax];

	int ret;
	asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (function));
	regs->eax = ret;
}