cd "%~dp0%"
cmd /c mount.bat
copy %1kernel.sys X:\Lemon\kernel.sys
cmd /c unmount.bat