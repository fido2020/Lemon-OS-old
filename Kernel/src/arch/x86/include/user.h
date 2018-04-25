#pragma once

#include <stdint.h>
#include <syscalls.h>

extern "C"
void usermode_enter();

void usermode_init();