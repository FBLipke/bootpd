/**
 * Keyboard I/O - BIOS INT 0x16
 * kbhit() - Check if key is available (returns 1 if key waiting, 0 if not)
 * getch() - Get character without echo (blocking)
 * getchar() - Alias for getch()
 */

#include <stdlib.h>

extern int _kbhit(void);
extern int _getch(void);

/* Check if a key is waiting in buffer */
int kbhit(void) {
    return _kbhit();
}

/* Get character without echo (blocking) */
int getch(void) {
    return _getch();
}

/* Alias */
int getchar(void) {
    return _getch();
}
