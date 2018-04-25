
cd src/arch/x86/

for i in *.asm
do
	echo Assembling $i
    nasm -f elf32 -g -F dwarf -o "../../../obj/x86/${i%.asm}.asm.o" "$i"
done

cd ../../../