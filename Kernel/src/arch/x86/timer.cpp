#include <timer.h>

// Timer callbacks
void(*callbacks[256])(uint16_t);
// Number of timer callbacks
uint8_t callbacks_num;

// Register timer callback
void timer_register_callback(void(*callback)(uint16_t)) {
	callbacks[callbacks_num++] = callback;
}

// Timer handler
void timer_handler(regs32_t *r) {
	for (int i = 0; i < callbacks_num; i++) {
		callbacks[i](18);
	}
}

// Install timer interrupt handler
void timer_install() {
	interrupt_register_handler(IRQ0, timer_handler);
}