#pragma once

#include <video.h>
#include <system.h>

void syscalls_init();
void syscall_handler(regs32_t* regs);