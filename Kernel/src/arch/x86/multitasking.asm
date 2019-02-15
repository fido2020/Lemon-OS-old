extern process_stack
extern process_base
extern process_entry_point
extern process_page_directory

global task_switch
global read_eip

read_eip:
    mov eax, [esp]
    ret

task_switch:
	cli
    mov ebx, [process_entry_point]
    mov esp, [process_stack]
    mov ebp, [process_base]
    mov eax, [process_page_directory]
    mov cr3, eax
	mov eax, 0xC0000123 ; Let the scheduler know the task has been switched
    sti
    jmp ebx