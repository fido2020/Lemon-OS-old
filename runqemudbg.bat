@echo off

%systemroot%\System32\bash -c "export DISPLAY=:0 ; qemu-system-i386 Disks/Lemon.vhd -s -S -vga std"
exit