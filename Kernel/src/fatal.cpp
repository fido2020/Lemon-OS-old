#include <fatal.h>
#include <video.h>
#include <console.h>
#include <serial.h>
#include <graphicsresources.h>

void fatal_error(char* msg, char* err) {
	write_serial_string(msg);
	write_serial('\n');
	write_serial_string(err);

<<<<<<< HEAD
	screen_clear(255, 0, 0);
	drawstring("Lemon has encountered a fatal error.", 2,10,255,255,255,2);
	drawstring("The system has been halted.", 2, 30, 255, 255, 255, 2);
	drawstring(msg, 2, 60, 255, 255, 255, 2);
	drawstring(err, 2, 80, 255, 255, 255, 2);

	drawgrayscalebitmap(2, 100, 20, 13, lemon_dead);
=======
	screen_clear_direct(255, 0, 0);
	drawstring_direct("Lemon has encountered a fatal error.", 2,10,255,255,255,1);
	drawstring_direct("The system has been halted.", 2, 30, 255, 255, 255, 1);
	drawstring_direct(msg, 2, 60, 255, 255, 255, 1);
	drawstring_direct(err, 2, 80, 255, 255, 255, 1);
>>>>>>> 17e9ca9a679e395e7e3bc93ec5eb2a2a0cd4790c

	screen_update();

	asm("cli");
	asm("hlt");

	for (;;);
}