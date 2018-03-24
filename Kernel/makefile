.PHONY: all clean

ARCH := x86

SRCDIRS := src src/arch/$(ARCH)
INCLUDEPATH := -I src/include -I src/arch/$(ARCH)/include

CFLAGS := -Wall -fno-exceptions -fno-rtti -std=c++14 -m32 -ffreestanding -Wno-write-strings $(INCLUDEPATH)
ASFLAGS := -f elf32 -g -F dwarf

CPPFILES := $(shell find $(SRCDIRS) -type f -name "\*.cpp")
ASMFILES := $(shell find $(SRCDIRS) -type f -name "\*.asm")
OBJFILES := $(patsubst %.cpp,%.cpp.o,$(CPPFILES)) $(patsubst %.asm,%.asm.o,$(ASMFILES))

all: $(OBJFILES)
	@echo $(CPPFILES)

%.cpp.o: %.cpp
	@g++ $(CFLAGS) -c $< -o $@

%.asm.o: %.asm
	@nasm $(ASFLAGS) -o $@ $<