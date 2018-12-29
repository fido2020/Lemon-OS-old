#include <keyboard.h>
#include <system.h>
#include <idt.h>
#include <serial.h>

static bool data_updated;
char key;
unsigned char key_raw;
bool caps;

unsigned char keymap_us[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};

// Interrupt handler
void keyboard_handler(regs32_t* r)
{

	// Read from the keyboard's data buffer
	key_raw = inportb(0x60);

	if (key_raw & 0x80) {
		// Shift, Ctrl, Alt, etc. is held down
	}
	else if(key_raw == 0x3A){
		caps = !caps;
	}
	else {
		data_updated = true;
		key = keymap_us[key_raw];
	}
}

// Register interrupt handler
void keyboard_install() {
	interrupt_register_handler(33, keyboard_handler);
}

// Check if key has been pressed since last check
bool key_updated() {
	bool u = data_updated;
	data_updated = false;
	return u;
}

//Get key stored in buffer
char get_key() {
	return key;
}

unsigned char get_key_raw() {
	return key_raw;
}

bool is_caps() {
	return caps;
}