#include <stdint.h>
#include <system.h>

#define MOUSE_X_SIGN (1 << 5)
#define MOUSE_Y_SIGN (1 << 6)

#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_RIGHT 2
#define MOUSE_BUTTON_MIDDLE 4

void mouse_install();
int8_t* mouse_get_data();
bool mouse_data_updated();