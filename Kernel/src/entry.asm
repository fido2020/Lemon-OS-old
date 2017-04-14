extern kload
global _entry

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)

section .multiboot

; Multiboot Header
MBALIGN		equ 1<<0
MEMINFO 	equ 1<<1
;VIDINFO		equ 1<<2
FLAGS		equ MBALIGN | MEMINFO; | VIDINFO
MAGIC		equ 0x1BADB002
CHECKSUM 	equ -(MAGIC + FLAGS)

align 4

dd MAGIC
dd FLAGS
dd CHECKSUM

section .text

STACKSIZE equ 0x4000

global entry

entry:

	;mov dword[BootPageDirectory], 0
	invlpg[0]

	cli
    cld
    mov esp, stack_top
	push eax

	;add ebx, KERNEL_VIRTUAL_BASE
	push ebx

	cli
	call kload

	cli
	hlt
hang:
	jmp hang

section .data
	align 0x1000
	BootPageDirectory:
	   dd 0x00000083
	   times (KERNEL_PAGE_NUMBER - 1) dd 0
	   dd 0x00000083
	   times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0


section .bss
align 32
stack:
	resb STACKSIZE
stack_top:
