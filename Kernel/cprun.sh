../mount.bat
cp bin/ia32/kernel.sys X:/Lemon/kernel.sys
../unmount.bat
cmd /c start "" "C:\Program Files\Oracle\VirtualBox\VBoxManage" startvm "Lemon"