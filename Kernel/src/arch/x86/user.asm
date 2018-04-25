global usermode_enter

usermode_enter:
	cli
	mov ax, 0x23
	
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push 0x23
	push esp
	pushfd
	push 0x1B
	lea eax, [s]

	iretd
s:
	add esp, 4