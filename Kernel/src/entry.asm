

extern kload

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

STACKSIZE equ 0x4000

global entry

entry:

	invlpg[0]

	cli
    cld
    mov esp, stack_top
	push eax
	
	push ebx
	
	;cli
	;call do_vbe
	
	cli
	call kload

	cli
	hlt
hang:
	jmp hang

	
section .bss
align 32
stack:
	resb STACKSIZE
stack_top: