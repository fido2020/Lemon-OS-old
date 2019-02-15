i686-elf-gcc -ffreestanding -nostdlib -lgcc -L../../API/bin -I../../API/include -T link.ld -Wno-int-conversion -o wm.elf wm.c -llemon
