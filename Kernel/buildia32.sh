#!/bin/bash

cd src

for i in *.cpp
do
	echo Compiling $i
    i686-elf-g++ -c "$i" -o "../obj/ia32/${i%.cpp}.cpp.o" -I include -I arch/ia32/include  --freestanding -fno-exceptions -fno-rtti -std=c++14 -Wno-write-strings -D ARCH_IA32
done

cd arch/ia32

for i in *.asm
do
	echo Assembling $i
    nasm -f elf32 -g -F dwarf -o "../../../obj/ia32/${i%.asm}.asm.o" "$i"
done

for i in *.cpp
do
	echo Compiling $i
    i686-elf-g++ -c "$i" -o "../../../obj/ia32/${i%.cpp}.cpp.o" -I ../include --freestanding -fno-exceptions -fno-rtti -std=c++14 -Wno-write-strings -D ARCH_IA32
done

cd ../../..

i686-elf-ld -m elf_i386 -T linkscript.ld -o bin/ia32/kernel.sys obj/ia32/*.asm.o obj/ia32/*.cpp.o
