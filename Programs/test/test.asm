section .text

global _start

_start:
    mov eax, 0
    mov ebx, test_string
    int 0x69
end:
    jmp end

section .data
test_string db "This is a test string"