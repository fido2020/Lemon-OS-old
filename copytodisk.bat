cd "%~dp0%"
cmd /c mount.bat
copy initrd.img X:\Lemon\initrd.img
copy %1kernel.sys X:\Lemon\kernel.sys
cmd /c unmount.bat