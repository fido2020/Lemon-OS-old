enter_usermode:
	cli
	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push 0x23
	push esp
	pushfd
	push 0x1b
	lea eax, [stk]
	push eax

	iretd
stk:
	add esp, 4