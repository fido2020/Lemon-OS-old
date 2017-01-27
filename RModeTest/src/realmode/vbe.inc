%ifndef VBE_INC_INCLUDED
%define VBE_INC_INCLUDED

VBE_PHYSICAL_BUFFER		equ 0xDF000000
VBE_BACK_BUFFER			equ 0xDF800000
DEFAULT_WIDTH			equ 800
DEFAULT_HEIGHT			equ 600

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
	
do_vbe:
	push es
	mov dword[vbe_info_block], "VBE2"
	mov ax, 0x4F00
	int 10h
	pop es
	
	cmp ax, 0x4F
	jne .no_vbe
	
	cmp dword[vbe_info_block.version], 0x200
	jl .old_vbe
	
	mov ax, [vbe_width]
	mov ax, [vbe_height]
	mov cl, 32
	call vbe_set_mode
	
	ret

.no_vbe:
	int 18h ; Print function doesen't work so just crash
	
.old_vbe:
	int 18h

vbe_set_mode:
	mov [.width], ax
	mov [.height], bx
	mov [.bpp], cl
	
	push es
	mov dword[vbe_info_block], "VBE2"
	mov ax, 4F00h
	mov di, vbe_info_block
	int 10h
	pop es
	
	cmp ax, 4Fh ; BIOS does not support VESA VBE
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
	mov fs,ax
	
	cmp word [.mode], 0xFFFF
	je .error
	
	push es
	mov ax, 4F01h ; Get VBE Mode Info
	mov cx, [.mode]
	mov di, mode_info_block
	int 10h
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
	
	test word [mode_info_block.attributes], 0x81
	jz .next_mode
	
	; Found Correct Mode - Set the Video Mode
	push es
	mov ax, 4F02h
	mov bx, [.mode]
	or bx, 4000h
	mov cx, 0
	mov dx, 0
	mov di, 0
	int 10h
	pop es
	
	cmp ax, 4Fh
	jne .error
	
	movzx eax, word [.width]
	mov [screen.width], eax
	movzx eax, word [.height]
	mov eax, [.height]
	movzx eax, byte [.bpp]
	mov [screen.bpp], eax
	add eax, 7
	shr eax, 3
	mov [screen.bytes_per_pixel], eax
	
	movzx eax, word [mode_info_block.pitch]
	mov [screen.bytes_per_line], eax
	
	mov eax, [mode_info_block.framebuffer]
	mov [screen.framebuffer], eax
	
	movzx eax, dword [.width]
	shr eax, 3
	dec eax
	mov [screen.x_max], eax
	
	movzx eax, dword [.height]
	shr eax, 4
	dec eax
	mov [screen.y_max], eax
	
	mov word [screen.x], 0
	mov word [screen.y], 0
	
	clc
	ret
	
.next_mode:
	mov ax, [.segment]
	mov fs, ax
	mov si, [.offset]
	jmp .find_mode
	
.error:
	int 18h
	
.width				dw 0
.height				dw 0
.bpp				db 0
.segment			dw 0
.offset				dw 0
.mode				dw 0
	
%endif