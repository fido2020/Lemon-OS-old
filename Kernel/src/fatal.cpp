#include <fatal.h>
#include <vga.h>
#include <console.h>
#include <serial.h>

#ifdef __cplusplus
extern "C"
#endif

using namespace VGA;

void fatal_error(char* msg, char* err) {
	/*clearscreen(0x44);
	puts("Lemon has encountered a fatal error. The system has been halted.\n\n", 0x4F);
	puts(msg, 0x4F);
	puts("\nError: ", 0x4F);
	puts(err, 0x4F);*/

	/*screen_clear(255, 0, 0);
	console::puts("Lemon has encountered a fatal error. The system has been halted.\n\n", 0x4F);
	console::puts(msg, 0x4F);
	console::puts("\nError: ", 0x4F);
	console::puts(err, 0x4F);*/

	write_serial_string(msg);

	for (;;);
}