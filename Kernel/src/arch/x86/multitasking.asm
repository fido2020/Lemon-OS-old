extern process_stack
extern process_entry_point

global execute_process_asm

execute_process_asm:
	push 0x10
	push dword [process_stack]
	push 0x200
	push 0x8
	push dword [process_entry_point]
	iretd