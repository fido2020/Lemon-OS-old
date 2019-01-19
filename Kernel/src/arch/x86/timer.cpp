#include <timer.h>

#include <multitasking.h>
#include <serial.h>

uint8_t sys_uptime_ticks;
float sys_uptime_seconds;
uint8_t sys_uptime_minutes;
uint32_t sys_uptime_hours;

uint32_t get_uptime_seconds(){
	return sys_uptime_seconds;
}

uint32_t get_uptime_minutes(){
	return sys_uptime_minutes;
}

uint32_t get_uptime_hours(){
	return sys_uptime_hours;
}

// Timer callbacks
void(*callbacks[256])(uint16_t);
// Number of timer callbacks
uint8_t callbacks_num = 0;

// Register timer callback
void timer_register_callback(void(*callback)(uint16_t)) {
	callbacks[callbacks_num++] = callback;
}

void scheduler_tick();

uint32_t frequency;

// Timer handler
void timer_handler(regs32_t *r) {
	/*sys_uptime_ticks++;
	if(sys_uptime_ticks >= 18){
		sys_uptime_seconds++;
		sys_uptime_ticks -= 18;
	}
	if(sys_uptime_seconds >= 60){
		sys_uptime_minutes++;
		sys_uptime_seconds -= 60;
	}
	if(sys_uptime_minutes >= 60){
		sys_uptime_hours++;
		sys_uptime_minutes -= 60;
	}*/
	scheduler_tick();
}

// Install timer interrupt handler
void timer_install(uint32_t freq) {
	frequency = freq;
	uint32_t divisor = 1193180 / freq;

	// Send the command byte.
	outportb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

	// Send the frequency divisor.
	outportb(0x40, l);
	outportb(0x40, h);

	interrupt_register_handler(IRQ0, timer_handler);
}