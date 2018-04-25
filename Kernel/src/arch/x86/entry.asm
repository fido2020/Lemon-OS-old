BITS    32

global entry
extern kmain

MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
VIDINFO		equ 1<<2
MAGIC       equ 0x1BADB002
FLAGS       equ MBALIGN | MEMINFO | VIDINFO
CHECKSUM    equ -(MAGIC + FLAGS)

<<<<<<< HEAD
section .multiboot
=======
section .text
>>>>>>> 17e9ca9a679e395e7e3bc93ec5eb2a2a0cd4790c
align 4 ; Multiboot Header
multiboot_header:
dd MAGIC
dd FLAGS
dd CHECKSUM
dd 0
dd 0
dd 0
dd 0
dd 0

dd 0
dd 0
dd 0
dd 32

section .text
entry:
<<<<<<< HEAD
=======

>>>>>>> 17e9ca9a679e395e7e3bc93ec5eb2a2a0cd4790c
    mov esp, stack_top
    push eax

    push ebx
    call kmain
    
    cli
    hlt

section .bss
align 4
stack_bottom:
resb 16384
stack_top: