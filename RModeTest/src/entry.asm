[BITS 16]
[ORG 0x0]
jmp entry

%include "realmode/gdt.inc"

entry:
cli

mov al, 0xdd
out 0x64, al

call LoadGDT

%define	VIDMEM	0xB8000		; video memory
 
mov	edi, VIDMEM		; get pointer to video memory
mov	edi, 'A'		; print character 'A'

cli
hlt