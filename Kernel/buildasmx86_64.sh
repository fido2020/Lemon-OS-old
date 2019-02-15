cd src/arch/x86_64/

for i in *.asm
do
	echo Assembling $i
    nasm -f elf64 -g -F dwarf -o "../../../obj/x86/${i%.asm}.asm.o" "$i"
done

cd ../../../