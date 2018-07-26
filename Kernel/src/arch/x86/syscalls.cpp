#include <syscalls.h>

#define NUM_SYSCALLS 1

typedef void(*syscall_t)(regs32_t*);

void test_syscall(regs32_t* r) {
	write_serial_string((char*)r->ebx);
}

syscall_t syscalls[1]{
	test_syscall
};


void syscalls_init() {
	interrupt_register_handler(0x69,syscall_handler);
}

void syscall_handler(regs32_t* regs) {
	if (regs->eax >= NUM_SYSCALLS) // If syscall is non-existant then return
		return;

	syscalls[regs->eax](regs); // Call syscall
}