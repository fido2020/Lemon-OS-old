#pragma once
#include <stdint.h>

typedef struct {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_num, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} regs32_t;

uint8_t inportb (uint16_t _port);
void outportb (uint16_t _port, uint8_t _data);