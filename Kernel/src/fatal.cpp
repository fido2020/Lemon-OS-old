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

	screen_clear_direct(255, 0, 0);
	drawstring_direct("Lemon has encountered a fatal error.", 2,10,255,255,255,1);
	drawstring_direct("The system has been halted.", 2, 30, 255, 255, 255, 1);
	drawstring_direct(msg, 2, 60, 255, 255, 255, 1);
	drawstring_direct(err, 2, 80, 255, 255, 255, 1);

	/*
	console::puts("Lemon has encountered a fatal error. The system has been halted.\n\n", 0x4F);
	console::puts(msg, 0x4F);
	console::puts("\nError: ", 0x4F);
	console::puts(err, 0x4F);*/

	asm("cli");
	asm("hlt");

	for (;;);
}