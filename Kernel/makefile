.PHONY: all clean

ARCH := x86

SRCDIRS := src/arch/$(ARCH) src src/liballoc src/gui
INCLUDEPATH := -I src/include -I src/arch/$(ARCH)/include

CFLAGS-x86 := -Wall -Wno-pointer-arith -fno-permissive -fno-exceptions -fno-rtti -std=c++14 -fno-pic -Wno-address -m32 -ffreestanding -Wno-write-strings -fno-stack-protector $(INCLUDEPATH)
ASFLAGS-x86 := -f elf32 -g -F dwarf

CFLAGS-x86_64 := -Wno-pointer-arith -Wall -fno-permissive -fno-exceptions -z max-page-size=0x1000 -fno-rtti -std=c++14 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -fno-pic -Wno-address -m64 -ffreestanding -Wno-write-strings -fno-stack-protector $(INCLUDEPATH)
ASFLAGS-x86_64 := -f elf64 -g -F dwarf

CFILES := $(shell find $(SRCDIRS) -type f -name "*.c")
CPPFILES := $(shell find $(SRCDIRS) -type f -name "*.cpp")
OBJFILES := $(patsubst %.cpp,%.cpp.o,$(CPPFILES)) $(patsubst % .asm,%.asm.o,$(ASMFILES)) $(patsubst %.c,%.c.o,$(CFILES))

ifeq ($(ARCH),x86)
CFLAGS=$(CFLAGS-x86)
endif

ifeq ($(ARCH),x86_64)
CFLAGS=$(CFLAGS-x86_64)
endif

.PHONY: all copy

all: $(ARCH)

x86: $(OBJFILES)
	@./buildasmx86.sh
	@i686-elf-g++ -m32 -T linkscript-x86.ld -o bin/$(ARCH)/kernel.sys -ffreestanding -O2 -nostdlib obj/x86/*.asm.o obj/x86/*.c.o obj/x86/*.cpp.o -fno-pic -lgcc 

x86_64: $(OBJFILES)
	@./buildasmx86_64.sh
	@g++ -m64 -T linkscript-x86_64.ld -o bin/$(ARCH)/kernel.sys -fno-pic -Wl,--build-id=none -ffreestanding -O2 -nostdlib obj/x86_64/*.asm.o obj/x86_64/*.c.o obj/x86_64/*.cpp.o -lgcc -fno-pic

copy:
	@cd .. && sudo ./copytodisk.sh

%.cpp.o: %.cpp
	@echo Compiling $<
	@i686-elf-g++ $(CFLAGS) -c $< -o obj/$(ARCH)/$(shell basename $@);

%.c.o: %.c
	@echo Compiling $<
	@i686-elf-gcc $(CFLAGS) -c $< -o obj/$(ARCH)/$(shell basename $@);

%.asm.o: %.asm
	@echo Assembling $<
	@nasm $(ASFLAGS) $< -o obj/$(ARCH)/$(shell basename $@);
