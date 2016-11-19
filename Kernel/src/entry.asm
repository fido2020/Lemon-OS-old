MBALIGN		equ 1<<0
MEMINFO 	equ 1<<1
VIDINFO		equ 1<<2
FLAGS		equ MBALIGN | MEMINFO ;| VIDINFO
MAGIC		equ 0x1BADB002
CHECKSUM 	equ -(MAGIC + FLAGS)

section .text
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM
;dd 0
;dd 0
;dd 0
;dd 0
;dd 0
;dd 0
;dd 800
;dd 600
;dd 32


global entry
extern kmain

entry:
	cli
	call kmain

	cli
	hlt
hang:
	jmp hang