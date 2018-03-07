#include <fatal.h>
#include <video.h>
#include <console.h>
#include <serial.h>

#ifdef __cplusplus
extern "C"
#endif

void fatal_error(char* msg, char* err) {
	write_serial_string(msg);
	write_serial('\n');
	write_serial_string(err);

	screen_clear(255, 0, 0);
	drawstring("Lemon has encountered a fatal error.", 2,10,255,255,255,2);
	drawstring("The system has been halted.", 2, 30, 255, 255, 255, 2);
	drawstring(msg, 2, 60, 255, 255, 255, 2);
	drawstring(err, 2, 80, 255, 255, 255, 2);

	screen_update();

	/*
	console::puts("Lemon has encountered a fatal error. The system has been halted.\n\n", 0x4F);
	console::puts(msg, 0x4F);
	console::puts("\nError: ", 0x4F);
	console::puts(err, 0x4F);*/

	asm("cli");
	asm("hlt");

	for (;;);
}