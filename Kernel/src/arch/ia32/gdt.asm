global gdt_flush
extern gdtp

gdt_flush:
    lgdt [gdtp]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:return
return:
    ret