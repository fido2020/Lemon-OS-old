#include <syscalls.h>

#include <video.h>

#define NUM_SYSCALLS 2

typedef void(*syscall_t)(regs32_t*);

void test_syscall(regs32_t* r) {
	write_serial_string((char*)r->ebx);
}

void test_syscall2(regs32_t* r) {
	screen_fillrect(0,0,500,400,0,0,0);
	screen_update();
}

syscall_t syscalls[NUM_SYSCALLS]{
	test_syscall,
	test_syscall2,
};


void syscalls_init() {
	interrupt_register_handler(0x69,syscall_handler);
}

void syscall_handler(regs32_t* regs) {
	if (regs->eax >= NUM_SYSCALLS) // If syscall is non-existant then return
		return;

	syscalls[regs->eax](regs); // Call syscall
}