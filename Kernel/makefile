all:
	${MAKE} build
	${MAKE} copy
	${MAKE} run

.PHONY: build
build:
	./build
.PHONY: clean
clean:
	rm obj/*.*

.PHONY: copy
copy:
	./copy

.PHONY: run
run:
	cmd /c start "" "C:\Program Files\Oracle\VirtualBox\VBoxManage" startvm "Lemon"
