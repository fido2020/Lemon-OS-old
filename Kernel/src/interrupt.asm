; Interrupt Service Routine (ISR)'s

global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

extern faultHandler

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
	mov eax, faultHandler
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
	_isr%1:
		cli
		push byte %1
		jmp isr_common_stub
%endmacro

%macro ISR_OS 1
	_isr%1:
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

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

%macro IRQ 1
	_irq%1:
		cli
		push byte 0
		push byte %1+32
		jmp irq_common_stub
%endmacro

extern IRQHandler

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
	mov eax, IRQHandler
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
