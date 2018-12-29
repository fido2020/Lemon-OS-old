extern process_stack
extern process_base
extern process_entry_point

global task_switch
global read_eip

read_eip:
	pop eax
	jmp eax

task_switch:
	cli
    mov ecx, [process_entry_point]
    mov esp, [process_stack]
    mov ebp, [process_base]
	mov eax, 0x6969 ; Let the scheduler know we switched task
    sti
    jmp ecx