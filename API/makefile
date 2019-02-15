.PHONY: all clean

ARCH := x86

SRCDIRS := src/gfx src/core src/io src/mem
INCLUDEPATH := -I include

CFLAGS-x86 := -fPIC -Wall -Wno-int-conversion -Wno-pointer-arith -fno-exceptions -Wno-address -m32 -ffreestanding -Wno-write-strings $(INCLUDEPATH)
ASFLAGS-x86 := -f elf32 -g -F dwarf

CFILES := $(shell find $(SRCDIRS) -type f -name "*.c")
OBJFILES := $(patsubst %.asm,%.asm.o,$(ASMFILES)) $(patsubst %.c,%.c.o,$(CFILES))

ifeq ($(ARCH),x86)
CFLAGS=$(CFLAGS-x86)
endif

ifeq ($(ARCH),x86_64)
CFLAGS=$(CFLAGS-x86_64)
endif

.PHONY: all copy initrd

all: $(ARCH)

x86: $(OBJFILES)
	@ar rcs bin/liblemon.a obj/*

%.c.o: %.c
	@echo Compiling $<
	@i686-elf-gcc $(CFLAGS) -c $< -o obj/$(shell basename $@);

%.asm.o: %.asm
	@echo Assembling $<
	@nasm $(ASFLAGS) $< -o obj/$(shell basename $@); 
