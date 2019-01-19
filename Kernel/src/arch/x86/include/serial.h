#pragma once

void init_serial();
void write_serial(char c);

#ifdef __cplusplus
extern "C"
#endif
void write_serial_string(char* str);