#include <hal.h>

extern "C"
void kmain_early(){
	initialize_hardware();
}
