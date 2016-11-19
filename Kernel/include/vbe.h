#include "int32.h"

void vbe_set_mode (int mode) {
   /* call BIOS */
   regs16_t in, out;
   in.eax.val = 0x4F02;
   in.ebx.val = mode;
   io_services (0x10, &in, &out);
}