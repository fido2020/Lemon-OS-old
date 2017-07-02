; Interrupt Service Routine (ISR)'s

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

extern isr_fault_handler

isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 10h
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov eax, esp
	push eax
	mov eax, isr_fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

%macro ISR_CPU 1
	isr%1:
		cli
		push byte %1
		jmp isr_common_stub
%endmacro

%macro ISR_OS 1
	isr%1:
		cli
		push byte 0
		push byte %1
		jmp isr_common_stub
%endmacro

ISR_OS  0
ISR_OS  1
ISR_OS  2
ISR_OS  3
ISR_OS  4
ISR_OS  5
ISR_OS  6
ISR_OS  7
ISR_CPU 8
ISR_OS  9
ISR_CPU 10
ISR_CPU 11
ISR_CPU 12
ISR_CPU 13
ISR_CPU 14
ISR_OS  15
ISR_OS  16
ISR_OS  17
ISR_OS  18
ISR_OS 19
ISR_OS 20
ISR_OS 21
ISR_OS 22
ISR_OS 23
ISR_OS 24
ISR_OS 25
ISR_OS 26
ISR_OS 27
ISR_OS 28
ISR_OS 29
ISR_OS 30
ISR_OS 31
ISR_OS 32

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

%macro IRQ 1
	irq%1:
		cli
		push byte 0
		push byte %1+32
		jmp irq_common_stub
%endmacro

extern irq_handler

irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov eax,esp
	push eax
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp,8
	iret

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
