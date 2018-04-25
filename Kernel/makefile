.PHONY: all clean

ARCH := x86

SRCDIRS := src/arch/$(ARCH) src src/liballoc src/util src/gui
INCLUDEPATH := -I src/include -I src/arch/$(ARCH)/include

CFLAGS := -Wall -fno-permissive -fno-exceptions -fno-rtti -std=c++14 -fno-pic -Wno-address -m32 -ffreestanding -Wno-write-strings -fno-stack-protector $(INCLUDEPATH)
ASFLAGS := -f elf32 -g -F dwarf

CFILES := $(shell find $(SRCDIRS) -type f -name "*.c")
CPPFILES := $(shell find $(SRCDIRS) -type f -name "*.cpp")
#ASMFILES := $(shell find $(SRCDIRS) -type f -name "*.asm")
OBJFILES := $(patsubst %.cpp,%.cpp.o,$(CPPFILES)) $(patsubst % .asm,%.asm.o,$(ASMFILES)) $(patsubst %.c,%.c.o,$(CFILES))

.PHONY: all copy

all: $(OBJFILES)
	@./buildasmx86.sh
	@g++ -m32 -T linkscript.ld -o bin/$(ARCH)/kernel.sys -fno-pic -Wl,--build-id=none -ffreestanding -O2 -nostdlib obj/x86/*.asm.o obj/x86/*.c.o obj/x86/*.cpp.o -lgcc -fno-pic

copy:
	@cd .. && sudo ./copytodisk.sh

%.cpp.o: %.cpp
	@echo Compiling $<
	@g++ $(CFLAGS) -c $< -o obj/$(ARCH)/$(shell basename $@);

%.c.o: %.c
	@echo Compiling $<
	@gcc $(CFLAGS) -c $< -o obj/$(ARCH)/$(shell basename $@);

%.asm.o: %.asm
	@echo Assembling $<
	@nasm $(ASFLAGS) $< -o obj/$(ARCH)/$(shell basename $@);