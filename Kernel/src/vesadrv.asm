;
;	VESA VBE Driver
;	Thanks to Michael Petch on Stack Overflow:
;	http://stackoverflow.com/users/3857942/michael-petch
;
;	VBE Code based off xOS:
;	https://github.com/omarrx024/xos
;
;	VBE Code License:
;	
;	MIT License
;
;	Permission is hereby granted, free of charge, to any person obtaining a copy
;	of this software and associated documentation files (the "Software"), to deal
;	in the Software without restriction, including without limitation the rights
;	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;	copies of the Software, and to permit persons to whom the Software is
;	furnished to do so, subject to the following conditions:
;
;	The above copyright notice and this permission notice shall be included in all
;	copies or substantial portions of the Software.
;
;	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;	SOFTWARE.
;

%include "gdt.inc"

VBE_PHYSICAL_BUFFER		equ 0xDF000000
VBE_BACK_BUFFER			equ 0xDF800000
DEFAULT_WIDTH			equ 1024
DEFAULT_HEIGHT			equ 768

global do_vbe

bits 16
section .data.realmode
save_idt: dw 0
          dd 0
save_esp: dd 0
real_ivt: dw (256 * 4) - 1      ; Realmode IVT has 256 CS:IP pairs
          dd 0                  ; Realmode IVT physical address at address 0x00000

align 32
vbe_width			dw DEFAULT_WIDTH
vbe_height			dw DEFAULT_HEIGHT

align 16			; Not sure if this has to be aligned, but it doesn't hurt.
vbe_info_block:
	.signature		db "VBE2"	; tell BIOS we support VBE 2.0+
	.version		dw 0
	.oem			dd 0
	.capabilities		dd 0
	.video_modes		dd 0
	.memory			dw 0
	.software_rev		dw 0
	.vendor			dd 0
	.product_name		dd 0
	.product_rev		dd 0
	.reserved:		times 222 db 0
	.oem_data:		times 256 db 0

align 16
mode_info_block:
	.attributes		dw 0
	.window_a		db 0
	.window_b		db 0
	.granularity		dw 0
	.window_size		dw 0
	.segmentA		dw 0
	.segmentB		dw 0
	.win_func_ptr		dd 0
	.pitch			dw 0

	.width			dw 0
	.height			dw 0

	.w_char			db 0
	.y_char			db 0
	.planes			db 0
	.bpp			db 0
	.banks			db 0

	.memory_model		db 0
	.bank_size		db 0
	.image_pages		db 0

	.reserved0		db 0

	.red			dw 0
	.green			dw 0
	.blue			dw 0
	.reserved_mask		dw 0
	.direct_color		db 0

	.framebuffer		dd 0
	.off_screen_mem		dd 0
	.off_screen_mem_size	dw 0
	.reserved1:		times 206 db 0

align 32
screen:
	.width			dd 0
	.height			dd 0
	.bpp			dd 0
	.bytes_per_pixel	dd 0
	.bytes_per_line		dd 0
	.screen_size		dd 0
	.screen_size_dqwords	dd 0	; in sets 8 DQWORDs, for SSE copying
	.framebuffer		dd 0
	.x			dd 0
	.y			dd 0
	.x_max			dd 0
	.y_max			dd 0
								
bits 32
section .text
do_vbe:
    pushad                      ; Save all the registers
    pushfd                      ; Save the flags (including Interrupt flag)
    cli
    mov [save_esp],esp
    sidt [save_idt]
    lidt [real_ivt]             ; We use a real ivt that points to the
                                ; physical address 0x00000 at the bottom of
                                ; memory. The IVT in real mode is 256*4 bytes
                                ; and runs from physical address 0x00000 to
                                ; 0x00400
    jmp CODE16SEL:pmode16

bits 16
section .text.realmode
pmode16:
    mov ax,DATA16SEL
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov eax,cr0
    dec eax
    mov cr0,eax
    jmp 0:realmode1

realmode1:
    mov ax,0x1000
    mov ss,ax
    xor sp,sp

    xor ax,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax

    push es					; some VESA BIOSes destroy ES, or so I read
	mov dword[vbe_info_block], "VBE2"
	mov ax, 0x4F00				; get VBE BIOS info
	mov di, vbe_info_block
	int 0x10
	pop es

	cmp ax, 0x4F
	jne .no_vbe

	cmp dword[vbe_info_block], "VESA"
	jne .no_vbe

	cmp word[vbe_info_block.version], 0x200
	jl .old_vbe

	; try 32bpp mode
	mov ax, [vbe_width]
	mov bx, [vbe_height]
	mov cl, 32
	call vbe_set_mode
	jc .bad_mode		; removed support for 24bpp modes...
    
    jmp done

.no_vbe:
	jmp done

.old_vbe:
	jmp done

.bad_mode:
	jmp done

; vbe_set_mode:
; Sets a VBE mode
; In\	AX = Width
; In\	BX = Height
; In\	CL = Bpp
; Out\	FLAGS.CF = 0 on success, 1 on error

vbe_set_mode:
	mov [.width], ax
	mov [.height], bx
	mov [.bpp], cl

	push es					; some VESA BIOSes destroy ES, or so I read
	mov dword[vbe_info_block], "VBE2"
	mov ax, 0x4F00				; get VBE BIOS info
	mov di, vbe_info_block
	int 0x10
	pop es

	cmp ax, 0x4F				; BIOS doesn't support VBE?
	jne .error

	mov ax, word[vbe_info_block.video_modes]
	mov [.offset], ax
	mov ax, word[vbe_info_block.video_modes+2]
	mov [.segment], ax

	mov ax, [.segment]
	mov fs, ax
	mov si, [.offset]

	.find_mode:
		mov dx, [fs:si]
		add si, 2
		mov [.offset], si
		mov [.mode], dx
		mov ax, 0
		mov fs, ax

		cmp word [.mode], 0xFFFF			; end of list?
		je .error

		push es
		mov ax, 0x4F01				; get VBE mode info
		mov cx, [.mode]
		mov di, mode_info_block
		int 0x10
		pop es

		cmp ax, 0x4F
		jne .error

		mov ax, [.width]
		cmp ax, [mode_info_block.width]
		jne .next_mode

		mov ax, [.height]
		cmp ax, [mode_info_block.height]
		jne .next_mode

		mov al, [.bpp]
		cmp al, [mode_info_block.bpp]
		jne .next_mode

		; does the mode support LFB and is it supported by hardware?
		test word [mode_info_block.attributes], 0x81
		jz .next_mode

		; if we make it here, we've found the correct mode!

		; set the mode!
		push es
		mov ax, 0x4F02
		mov bx, [.mode]
		or bx, 0x4000
		mov cx, 0
		mov dx, 0
		mov di, 0
		int 0x10
		pop es

		cmp ax, 0x4F
		jne .error

		; save the mode information
		movzx eax, word[.width]
		mov [screen.width], eax
		movzx eax, word[.height]
		mov [screen.height], eax
		movzx eax, byte[.bpp]
		mov [screen.bpp], eax
		add eax, 7
		shr eax, 3
		mov [screen.bytes_per_pixel], eax

		movzx eax, word [mode_info_block.pitch]
		mov [screen.bytes_per_line], eax

		mov eax, [mode_info_block.framebuffer]
		mov [screen.framebuffer], eax

		movzx eax, word[.width]
		shr eax, 3		; div 8
		dec eax
		mov [screen.x_max], eax

		movzx eax, word[.height]
		shr eax, 4		; div 16
		dec eax
		mov [screen.y_max], eax

		mov dword[screen.x], 0
		mov dword[screen.y], 0

		clc
		jmp done

	.next_mode:
		mov ax, [.segment]
		mov fs, ax
		mov si, [.offset]
		jmp .find_mode

	.error:
		mov ax, 0
		mov fs, ax
		stc
		jmp done

	.width				dw 0
	.height				dw 0
	.bpp				db 0
	.segment			dw 0
	.offset				dw 0
	.mode				dw 0
	
done:
    cli
    mov eax,cr0
    inc eax
    mov cr0,eax
    jmp dword CODE32SEL:pm1     ; To FAR JMP to address > 0xFFFF we need
                                ; to specify DWORD to allow a 32-bit address
                                ; in the offset portion. When this JMP is
                                ; complete CS will be CODE32SEL and processor
                                ; will be in 32-bit protected mode

bits 32
section .text
pm1:
    mov eax,DATA32SEL
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov dword esp,[save_esp]
    lidt [save_idt]
    popfd                       ; Restore flags (including Interrupt flag)
    popad                       ; Restore registers

    mov eax, mode_info_block    ; Return pointer to mode_info structure
    ret

