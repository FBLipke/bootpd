/**
 * PXE Call Stub - For Linux/non-RealMode builds
 * Returns mock success (Status = 0)
 */

#ifndef _MSC_VER
#include <stdlib.h>

/* PXE API call stub for non-RealMode builds */
uint16_t _pxe_call(uint16_t func, uint16_t bx, uint16_t cx, uint16_t dx, uint16_t di, uint16_t si) {
    (void)func; (void)bx; (void)cx; (void)dx; (void)di; (void)si;
    return 0;  /* Return success status in AX */
}

/* Boot jump stub */
void _boot_jump(void) {
    /* Nothing to do in stub */
}
#endif
