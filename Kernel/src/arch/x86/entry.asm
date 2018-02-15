BITS    32

global entry
extern kmain

MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
VIDINFO		equ 1<<2
MAGIC       equ 0x1BADB002
FLAGS       equ MBALIGN | MEMINFO | VIDINFO
CHECKSUM    equ -(MAGIC + FLAGS)

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ KERNEL_VIRTUAL_BASE >> 22
KERNEL_NUM_PAGES equ 3

section .multiboot.data
align 4096
KernelPageDirectory:
	dd 0x00000083
    times (KERNEL_PAGE_NUMBER - 1) dd 0			; Pages before kernel space
    dd 0x00000083 | 0<<22
    dd 0x00000083 | 1<<22
	dd 0x00000083 | 2<<22
 	dd 0x00000083 | 3<<22
    times (1024 - KERNEL_PAGE_NUMBER - KERNEL_NUM_PAGES) dd 0	; Pages after kernel space


section .multiboot.text
;section .text
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

entry:
	mov ecx, (KernelPageDirectory)
	mov cr3, ecx

	mov ecx, cr4
	or ecx, 0x00000010
	mov cr4, ecx

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

 	add ebx, KERNEL_VIRTUAL_BASE
    push ebx
    call kmain ; Load C++ part of kernel
    
    cli
    hlt

section .bss
align 4
stack_bottom:
resb 16384
stack_top: