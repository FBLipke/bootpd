/**
 * print_dec - Print values in decimal
 */

#include <stdlib.h>

extern void _print_char(char c);

void print_dec16(uint16_t val) {
    char buf[6];
    int i = 0;
    if (val == 0) {
        _print_char('0');
        return;
    }
    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    while (i--) _print_char(buf[i]);
}

void print_dec32(uint32_t val) {
    char buf[11];
    int i = 0;
    if (val == 0) {
        _print_char('0');
        return;
    }
    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    while (i--) _print_char(buf[i]);
}
