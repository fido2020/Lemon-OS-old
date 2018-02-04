BITS    32

global entry
extern kmain

MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
MAGIC       equ 0x1BADB002
FLAGS       equ MBALIGN | MEMINFO
CHECKSUM    equ -(MAGIC + FLAGS)

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ KERNEL_VIRTUAL_BASE >> 22

section .mutliboot.data
align 4096
KernelPageDirectory:
	dd 0x00000083
    times (KERNEL_PAGE_NUMBER - 1) dd 0			; Pages before kernel space
    dd 0x00000083
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0	; Pages after kernel space

section .multiboot.hdr
align 4 ; Multiboot Header
multiboot_header:
dd MAGIC
dd FLAGS
dd CHECKSUM

section .multiboot.text progbits alloc exec nowrite align=16
entry:
	mov ecx, (KernelPageDirectory - KERNEL_VIRTUAL_BASE)
	mov cr3, ecx

	mov ecx, cr4
	or ecx, 0x00000010
	mov cr0, ecx

	mov ecx, cr0
    or ecx, 0x80000000                 ; Set PG bit to enable paging.
    mov cr0, ecx

	lea ecx, [entry_higher_half]
	jmp ecx ; Jump to higher half

section .text
entry_higher_half:
	mov dword [KernelPageDirectory],0
	invlpg [0]

    mov esp, stack_top
    push eax

	;add ebx, KERNEL_VIRTUAL_BASE
    ;push ebx
    ;call kmain ; Load C++ part of kernel
    
    cli
    hlt

section .bss
align 4
stack_bottom:
resb 16384
stack_top: