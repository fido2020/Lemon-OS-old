%ifndef GDT_INC_INCLUDED
%define GDT_INC_INCLUDED

LoadGDT:
 
	cli				; clear interrupts
	pusha				; save registers
	lgdt 	[gdt_ptr]			; load GDT into GDTR
	sti				; enable interrupts
	popa				; restore registers
	ret	

gdt_data:
	; Null Descriptor
	dd 0
	dd 0
	
	; GDT Code
	dw 0FFFFh		; Limit
	dw 0 			; Base Low
	db 0 			; Base Middle
	db 10011010b	; Access
	db 11001111b	; Granularity
	db 0			; Base High
	
	; GDT Data
	dw 0FFFFh		; Limit
	dw 0			; Base Low
	db 0			; Base Middle
	db 10010010b	; Access
	db 11001111b	; Granularity
	db 0			; Base High
	
end_of_gdt:
gdt_ptr: ; Points to GDT
	dw end_of_gdt - gdt_data - 1
	dd gdt_data
	
%endif