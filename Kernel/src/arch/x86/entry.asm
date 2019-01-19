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
KERNEL_PAGE_DIR_NUMBER equ 768;(KERNEL_VIRTUAL_BASE >> 22)

section .multiboot.hdr
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


section .multiboot.data
;align 0x1000
;boot_page_directory: ; Create a temporary PSE (quick and easy to set up) page directory until we go to the higher half
;	dd 0x00000083
;    times (KERNEL_PAGE_DIR_NUMBER - 1) dd 0                 ; Pages before kernel space.
;    ; This page directory entry defines a 4MB page containing the kernel.
;    dd 0x00000083
;	dd 0x00400083
;	
;    times (1024 - KERNEL_PAGE_DIR_NUMBER - 12) dd 0  ; Pages after the kernel image.

;boot_page_table
align 0x1000
boot_page_directory:
	times 1024 dd 0

boot_page_table1:
	times 1024 dd 0

boot_page_table2:
	times 1024 dd 0

section .multiboot.text
entry:

	mov edi, boot_page_table1
	mov ecx, 2048
	mov esi, 0

next_page:

	mov edx, esi
	or edx, 0x003
	mov [edi], edx

	add esi, 4096
	add edi, 4

	loop next_page

	mov dword [boot_page_directory], (boot_page_table1 + 0x003)
	mov dword [boot_page_directory + 4], (boot_page_table2 + 0x003)
	mov dword [boot_page_directory + (KERNEL_PAGE_DIR_NUMBER * 4)], (boot_page_table1 + 0x003)
	mov dword [boot_page_directory + (KERNEL_PAGE_DIR_NUMBER * 4) + 4], (boot_page_table2 + 0x003)

	mov ecx, boot_page_directory ; Load page directory
	mov cr3, ecx
	
	;mov ecx, cr4
	;or ecx, 0x00000010 ; Enable PSE
	;mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000 ; Enable Paging
	mov cr0, ecx

	lea ecx, [entry_hh]
	jmp ecx

section .text
entry_hh:
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

	add ebx, KERNEL_VIRTUAL_BASE
    push ebx
    call kmain
    
    cli
    hlt

section .bss
align 32
stack_bottom:
resb 16384
stack_top: