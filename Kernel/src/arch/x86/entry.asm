BITS    32

global entry
extern kmain

MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
VIDINFO		equ 1<<2
MAGIC       equ 0x1BADB002
FLAGS       equ MBALIGN | MEMINFO | VIDINFO
CHECKSUM    equ -(MAGIC + FLAGS)

section .multiboot
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
    mov esp, stack_top
    push eax

	; Enable SSE
	mov eax, cr4
	or eax, 3 << 9 ; Set bits 9 & 10 in cr4
	mov cr4, eax
	;mov eax, cr0
	;and ax, 0xFFFB		;clear coprocessor emulation
	;or ax, 0x2			;set coprocessor monitoring
	;mov cr0, eax
	;mov eax, cr4
	;or ax, 3 << 9		; set flags for sse
	;mov cr4, eax

    push ebx
    call kmain
    
    cli
    hlt

section .bss
align 4
stack_bottom:
resb 16384
stack_top: