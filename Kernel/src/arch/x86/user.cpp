#include <user.h>

#include <stdint.h>
#include <gdt.h>
#include <memory.h>

void usermode_init() {
	syscalls_init();
}