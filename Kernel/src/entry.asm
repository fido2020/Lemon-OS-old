global _entry
extern kmain

; Multiboot Header
MBALIGN		equ 1<<0
MEMINFO 	equ 1<<1
VIDINFO		equ 1<<2
FLAGS		equ MBALIGN | MEMINFO | VIDINFO
MAGIC		equ 0x1BADB002
CHECKSUM 	equ -(MAGIC + FLAGS)

section .text
align 4

dd MAGIC
dd FLAGS
dd CHECKSUM
dd 0
dd 0
dd 0
dd 0
dd 0
dd 0
dd 800
dd 600
dd 32

STACKSIZE equ 0x4000

global entry

entry:
	

	mov esp, stack+STACKSIZE
	push eax
	
	push ebx
	
	cli
	call kmain

	cli
	hlt
hang:
	jmp hang
	
section .bss
align 32
stack:
	resb STACKSIZE