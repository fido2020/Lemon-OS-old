BITS	32

MBALIGN		equ 1<<0
MEMINFO		equ 1<<1
;VIDINFO	equ 1<<2 ; Set when using graphical mode
MAGIC		equ 0x1BADB002
FLAGS		equ MBALIGN | MEMINFO; | VIDINFO
CHECKSUM	equ -(MAGIC + FLAGS)

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

global entry
extern kmain_early

section .text
entry:
	mov esp, stack_top
	push eax

	push ebx
	call kmain_early

	cli
	hlt

section .bss
align 4
stack_bottom:
resb 16384 ; 16 KB
stack_top:
