CPPCIA32 = i686-elf-g++
CPPCIA32FLAGS = -I ../include --freestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -D ARCH_IA32

ASSEMBLER = nasm
ASFLAGS = -g -F dwarf

ASIA32FMT = elf32

ia32runqemu: ia32
	qemu-system-i386  --kernel bin/ia32/kernel.sys

runvbox:
	cmd /c start "" "C:\Program Files\Oracle\VirtualBox\VBoxManage" startvm "Lemon"

ia32:
	./buildia32.sh
