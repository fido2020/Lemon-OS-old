#pragma once

#include <idt.h>
#include <serial.h>

void syscalls_init();
void syscall_handler(regs32_t* regs);